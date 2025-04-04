import pandas as pd

df = pd.read_csv("data/raw/202401-citibike-tripdata.csv", parse_dates=["started_at"])
df = df.dropna(subset=["start_station_id", "start_lat", "start_lng", "started_at"])
df["hour"] = df["started_at"].dt.hour

hourly_usage = (
    df.groupby(["hour", "start_station_id"])
    .size()
    .reset_index(name="ride_count")
)

station_info = (
    df.drop_duplicates(subset=["start_station_id"])
    [["start_station_id", "start_station_name", "start_lat", "start_lng"]]
    .dropna()
    .reset_index(drop=True)
)

hourly_usage.to_csv("data/processed/hourly_usage.csv", index=False)
station_info.to_csv("data/processed/station_info.csv", index=False)
