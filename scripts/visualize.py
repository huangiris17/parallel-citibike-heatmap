import pandas as pd
import numpy as np
import folium
from folium.plugins import HeatMapWithTime

count = pd.read_csv("data/aggregated/hourly_usage_par.csv")
station_lookup = pd.read_csv("data/processed/station_info.csv")
merged = count.merge(station_lookup, on = "start_station_id", how = "left")

merged = merged.dropna(subset = ["hour", "start_lat", "start_lng", "ride_count"])
merged["hour"] = merged["hour"].astype(int)
merged["scaled_count"] = np.sqrt(merged["ride_count"])
merged["scaled_count"] /= merged["scaled_count"].max()

heat_data = []
for hr in range(24):
    data_per_hr = merged[merged["hour"] == hr]
    data_points = [
        [row["start_lat"], row["start_lng"], row["scaled_count"]]
        for _,row in data_per_hr.iterrows()
    ]
    heat_data.append(data_points)

time_index = [f"{hour:02d}:00" for hour in range(24)]

map = folium.Map(location = [40.75, -73.98], zoom_start = 13)

HeatMapWithTime(heat_data,
                index = time_index,
                auto_play = False,
                radius = 10,
                max_opacity = 0.9).add_to(map)

map.save("nyc_citibike_heatmap.html")