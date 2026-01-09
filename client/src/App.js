import React, { useEffect, useState } from 'react';

const serverUrl = process.env.REACT_APP_SERVER;

function App() {
  const [data, setData] = useState(null);

  useEffect(() => {
    const fetchData = async () =>{
      try {
        const response = await fetch(`${serverUrl}/api/sensor`);
        const json = await response.json();
        setData(json);
      } catch (error) {
        console.error("Error fetching sensor data:", error);
      }
    };
    fetchData();
    const interval = setInterval(fetchData, 5000);
    return () => clearInterval(interval);
      
  }, []);

  return (<div className="App">  
    <h1>Sensor Dashboard</h1>
    <p>Temp: {data ? data.temp + '°C' : "Loading..."}</p>
    <p>Hum: {data ? data.hum + '%' : "Loading..."}</p>
    <p>Pres: {data ? data.pres + 'hPa' : "Loading..."}</p>
  </div>);
}

export default App;