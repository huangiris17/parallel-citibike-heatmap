import pandas as pd
import glob
import os

csv_files = glob.glob("data/raw/*.csv")

df_list = []
for file in csv_files:
    df = pd.read_csv(file, parse_dates=["started_at"])
    df = df.dropna(subset=["start_station_id", "start_lat", "start_lng", "started_at"])
    df["hour"] = df["started_at"].dt.hour
    df_list.append(df)

df_combined = pd.concat(df_list, ignore_index=True)

station_info = (
    df_combined.drop_duplicates(subset=["start_station_id"])
    [["start_station_id", "start_station_name", "start_lat", "start_lng"]]
    .dropna()
    .reset_index(drop=True)
)

station_info.to_csv("data/processed/station_info.csv", index=False)

df_combined = df_combined[["hour", "start_station_id"]]
df_combined.to_csv("data/processed/hourly_usage.csv", index=False)
