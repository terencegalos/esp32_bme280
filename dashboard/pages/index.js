import { useState, useEffect } from 'react';

export default function Home() {
  const [data, setData] = useState({ temp: 0, humidity: 0, pressure: 0 });

  useEffect(() => {
    // Placeholder: Fetch data from ESP32 API
    // fetch('/api/sensor').then(res => res.json()).then(setData);
  }, []);

  return (
    <div>
      <h1>ESP32 Sensor Dashboard</h1>
      <p>Temperature: {data.temp}°C</p>
      <p>Humidity: {data.humidity}%</p>
      <p>Pressure: {data.pressure} hPa</p>
    </div>
  );
}
