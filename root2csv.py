import os
import uproot
import pandas as pd

root_folder = os.path.expanduser("~/maybe_caeminate/output")
project_folder = os.path.dirname(root_folder)

csv_folder = project_folder
os.makedirs(csv_folder, exist_ok=True)

metadata_list = []
summary_list = []

print("\nStarting ML-v0 extraction...\n")

# ---------------------------
# LOAD DATA
# ---------------------------

for filename in os.listdir(root_folder):
    if not filename.endswith(".root"):
        continue

    root_path = os.path.join(root_folder, filename)
    file = uproot.open(root_path)

    print(f"\nProcessing: {filename}")

    for key in file.keys():
        tree_name = key.split(";")[0]

        if not hasattr(file[key], "arrays"):
            continue

        try:
            arrays = file[key].arrays(library="np")
            df = pd.DataFrame(arrays)

            if df.empty:
                print(f"   Skipped empty tree: {tree_name}")
                continue

            df["SourceFile"] = filename

            if tree_name == "Metadata":
                metadata_list.append(df)
                print(f"   Loaded Metadata ({len(df)})")

            elif tree_name == "Summary":
                summary_list.append(df)
                print(f"   Loaded Summary ({len(df)})")

            elif tree_name == "Hits":
                print("   Ignored Hits")

            else:
                print(f"   Ignored tree: {tree_name}")

        except Exception as e:
            print(f"   Failed {tree_name}: {e}")

# ---------------------------
# BUILD DATASET
# ---------------------------

if not metadata_list:
    raise RuntimeError("No Metadata trees found.")

df_meta = pd.concat(metadata_list, ignore_index=True)
df_meta["RunID"] = pd.to_numeric(df_meta["RunID"], errors="coerce")
df_meta = df_meta.drop_duplicates("RunID")

if summary_list:
    df_summary = pd.concat(summary_list, ignore_index=True)
    df_summary["RunID"] = pd.to_numeric(df_summary["RunID"], errors="coerce")
    df_summary = df_summary.drop_duplicates("RunID")

    df_meta = df_meta.merge(
        df_summary,
        on="RunID",
        how="left",
        suffixes=("", "_summary")
    )

# ---------------------------
# STACK FLATTENING
# ---------------------------

def split_stack(s, n=5):
    if pd.isna(s):
        return [None] * n

    parts = str(s).split("|")
    return (parts + [None] * n)[:n]

if "LayerStack" in df_meta.columns:
    layers = df_meta["LayerStack"].apply(split_stack).apply(pd.Series)
    layers.columns = [f"L{i+1}" for i in range(5)]
    df_meta = pd.concat([df_meta, layers], axis=1)

if "ThicknessStack" in df_meta.columns:
    thick = df_meta["ThicknessStack"].apply(split_stack).apply(pd.Series)
    thick.columns = [f"T{i+1}" for i in range(5)]
    df_meta = pd.concat([df_meta, thick], axis=1)

if "DensityStack" in df_meta.columns:
    dens = df_meta["DensityStack"].apply(split_stack).apply(pd.Series)
    dens.columns = [f"D{i+1}" for i in range(5)]
    df_meta = pd.concat([df_meta, dens], axis=1)

# ---------------------------
# SAVE
# ---------------------------

csv_path = os.path.join(csv_folder, "data.csv")
df_meta.to_csv(csv_path, index=False)

print("\nSaved ML dataset:", csv_path)
print("Final rows:", len(df_meta))

assert df_meta["RunID"].is_unique, "RunID duplication detected!"

print("\nDone ✔ ML dataset ready")