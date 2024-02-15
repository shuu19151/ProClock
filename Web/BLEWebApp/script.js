// DOM Elements
const connectBleButton = document.getElementById('connectBleButton');
const disconnectBleButton = document.getElementById('disconnectBleButton');
const connectWifiButton = document.getElementById('connectWifiButton');
const disconnectWifiButton = document.getElementById('disconnectWifiButton');
const appSelectionButton = document.getElementById('appSelectionButton');
const wifiSendButton = document.getElementById('wifiSendButton');
const cryptoSendButton = document.getElementById('cryptoSendButton');
const retrievedValue = document.getElementById('valueContainer');
const latestValueSent = document.getElementById('valueSent');
const bleStateContainer = document.getElementById('bleState');
const wifiStateContainer = document.getElementById('wifiState');
const wifiSsid = document.getElementById('wifissid');
const wifiPass = document.getElementById('wifipass');
const baseCurrency = document.getElementById('basecurrency');
const crypto = document.getElementById('crypto');
const appSelection = document.getElementById('AppSelection');

//Define BLE Device Specs
var deviceName ='ESP32-ProClock';
var bleService = 'fc96f65e-318a-4001-84bd-77e9d12af44b';
var clientTXCharacteristic = '04d3552e-b9b3-4be6-a8b4-aa43c4507c4d';
var clientRXCharacteristic = '94b43599-5ea2-41e7-9d99-6ff9b904ae3a';

//Global Variables to Handle Bluetooth
var bleServer;
var bleServiceFound;
var clientRXCharacteristicFound;

// Connect BLE Button (search for BLE Devices only if BLE is available)
connectBleButton.addEventListener('click', (event) => {
    if (isWebBluetoothEnabled()){
        connectToDevice();
    }
});
// Disconnect BLE Button
disconnectBleButton.addEventListener('click', disconnectDevice);

// Connect Wifi Button
connectWifiButton.addEventListener('click', async () => {
    await writeOnCharacteristic(clientTXCharacteristic, '!' + '1');
});

// Disconnect Wifi Button
disconnectWifiButton.addEventListener('click', async () => {
    await writeOnCharacteristic(clientTXCharacteristic, '!' + '0');
});

// App Selection Button
appSelectionButton.addEventListener('click', async () => {
    var appSelectionValue = appSelection.options[appSelection.selectedIndex].value;
    await writeOnCharacteristic(clientTXCharacteristic, '"' + appSelectionValue);
});

// Wifi Send Button
wifiSendButton.addEventListener('click', async () => {
    await writeOnCharacteristic(clientTXCharacteristic, '#' + wifiSsid.value);
    await writeOnCharacteristic(clientTXCharacteristic, '$' + wifiPass.value);
});
cryptoSendButton.addEventListener('click', async () => {
    var baseCurrencyValue   = baseCurrency.options[baseCurrency.selectedIndex].value;
    var cryptoValue         = crypto.options[crypto.selectedIndex].value;
    await writeOnCharacteristic(clientTXCharacteristic, '%' + baseCurrencyValue);
    await writeOnCharacteristic(clientTXCharacteristic, '&' + cryptoValue);
});

// Check if BLE is available in your Browser
function isWebBluetoothEnabled() {
    if (!navigator.bluetooth) {
        console.log('Web Bluetooth API is not available in this browser!');
        bleStateContainer.innerHTML = "Web Bluetooth API is not available in this browser/device!";
        return false
    }
    console.log('Web Bluetooth API supported in this browser.');
    return true
}

// Connect to BLE Device and Enable Notifications
function connectToDevice(){
    console.log('Initializing Bluetooth...');
    navigator.bluetooth.requestDevice({
        filters: [{name: deviceName}],
        optionalServices: [bleService]
    })
    .then(device => {
        console.log('Device Selected:', device.name);
        bleStateContainer.innerHTML = 'Connected to device ' + device.name;
        bleStateContainer.style.color = "#24af37";
        device.addEventListener('gattservicedisconnected', onDisconnected);
        return device.gatt.connect();
    })
    .then(gattServer =>{
        bleServer = gattServer;
        console.log("Connected to GATT Server");
        return bleServer.getPrimaryService(bleService);
    })
    .then(service => {
        bleServiceFound = service;
        console.log("Service discovered:", service.uuid);
        return service.getCharacteristic(clientRXCharacteristic);
    })
    .then(characteristic => {
        console.log("Characteristic discovered:", characteristic.uuid);
        clientRXCharacteristicFound = characteristic;
        characteristic.addEventListener('characteristicvaluechanged', serverNotifyHandler);
        characteristic.startNotifications();
        console.log("Notifications Started.");
        return characteristic.readValue();
    })
    .catch(error => {
        console.log('Error: ', error);
    })
}

function onDisconnected(event){
    console.log('Device Disconnected:', event.target.device.name);
    bleStateContainer.innerHTML = "Device disconnected";
    bleStateContainer.style.color = "#d13a30";
    connectToDevice();
}

function serverNotifyHandler(event){
    const newValueReceived = new TextDecoder().decode(event.target.value);
    console.log("Characteristic value changed: ", newValueReceived);
    if(newValueReceived.startsWith('!')) {
        if(newValueReceived.endsWith('1')) {
            wifiStateContainer.innerHTML = "Wifi Connected";
            wifiStateContainer.style.color = "#24af37";
        } else if (newValueReceived.endsWith('0')) {
            wifiStateContainer.innerHTML = "Wifi Disconnected";
            wifiStateContainer.style.color = "#d13a30";
        }
    }
}

async function writeOnCharacteristic(Characteristic, string){
    if (bleServer && bleServer.connected) {
        try {
            // Get the characteristic
            let characteristic = await bleServiceFound.getCharacteristic(Characteristic);
            console.log("Found the Wifi characteristic: ", characteristic.uuid);

            // Encode the string value
            let encoder = new TextEncoder('utf-8');
            let data = encoder.encode(string);

            // Write the value with response
            await characteristic.writeValueWithoutResponse(data);
            console.log("Value written to Wifi characteristic:", string);
        } catch (error) {
            // Handle any errors
            console.error("Error writing to the Wifi characteristic: ", error);
        }
    } else {
        console.error ("Bluetooth is not connected. Cannot write to characteristic.")
        window.alert("Bluetooth is not connected. Cannot write to characteristic. \n Connect to BLE first!")
    }
}

function disconnectDevice() {
    console.log("Disconnect Device.");
    if (bleServer && bleServer.connected) {
        if (clientRXCharacteristicFound) {
            clientRXCharacteristicFound.stopNotifications()
                .then(() => {
                    console.log("Notifications Stopped");
                    return bleServer.disconnect();
                })
                .then(() => {
                    console.log("Device Disconnected");
                    bleStateContainer.innerHTML = "Device Disconnected";
                    bleStateContainer.style.color = "#d13a30";
    
                    wifiStateContainer.innerHTML = "Wifi Disconnected";
                    wifiStateContainer.style.color = "#d13a30";

                })
                .catch(error => {
                    console.log("An error occurred:", error);
                });
        } else {
            console.log("No characteristic found to disconnect.");
        }
    } else {
        // Throw an error if Bluetooth is not connected
        console.error("Bluetooth is not connected.");
        window.alert("Bluetooth is not connected.")
    }
}
