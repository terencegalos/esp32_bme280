const express = require('express');
const cors = require('cors');
const os = require('os');
const interfaces = os.networkInterfaces();

let serverIp = '';

for (const iface of Object.values(interfaces)) {
    for (const config of iface) {
        if (config.family ==='IPv4' && !config.internal) {
            console.log(`Server IP Address: ${config.address}`);
            serverIp = config.address;
        }
    }
}

const app = express();
app.use(cors());
app.use(express.json());

let data = {};

app.post('/api/sensor', (req, res) => {
    data = req.body;
    res.status(200).json({ message: 'Received', data: data});
});

app.get('/api/sensor', (req, res) => res.json(data));

const port = 3030;
app.listen(port, '0.0.0.0', () => console.log(`Server running on http://0.0.0.0:${port}`));