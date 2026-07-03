import os
import uproot
import pandas as pd

root_folder = os.path.expanduser("~/maybe_caeminate/output")
project_folder = os.path.dirname(root_folder)

csv_folder = os.path.join(project_folder, "")
os.makedirs(csv_folder, exist_ok=True)

all_dfs = []

for filename in os.listdir(root_folder):
    if not filename.endswith(".root"):
        continue

    root_path = os.path.join(root_folder, filename)
    file = uproot.open(root_path)

    print(f"\nProcessing: {filename}")

    for key in file.keys():

        obj = file[key]

        if not hasattr(obj, "arrays"):
            continue

        try:
            df = obj.arrays(library="pd")

            if df is None or len(df) == 0:
                print(f"   Skipped empty tree: {key}")
                continue

            # Optional: keep track of where each row came from
            df["SourceFile"] = filename
            df["Tree"] = key.replace(";", "")

            all_dfs.append(df)

            print(f"   Loaded {key}")

        except Exception as e:
            print(f"   Failed {key}: {e}")

# Combine everything into one CSV
if all_dfs:
    combined_df = pd.concat(all_dfs, ignore_index=True)

    csv_path = os.path.join(csv_folder, "data.csv")
    combined_df.to_csv(csv_path, index=False)

    print(f"\n Saved combined CSV: {csv_path}")
    print(f"Total rows: {len(combined_df)}")
else:
    print("\nNo data found to save.")

print("\nDone!")