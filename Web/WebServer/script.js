// DOM Elements
// const connectWifiButton = document.getElementById('connectWifiButton');
// const disconnectWifiButton = document.getElementById('disconnectWifiButton');
// const appSelectionButton = document.getElementById('appSelectionButton');
// const wifiSendButton = document.getElementById('wifiSendButton');
// const cryptoSendButton = document.getElementById('cryptoSendButton');
// const retrievedValue = document.getElementById('valueContainer');
// const latestValueSent = document.getElementById('valueSent');
// const wifiStateContainer = document.getElementById('wifiState');
// const wifiSsid = document.getElementById('wifissid');
// const wifiPass = document.getElementById('wifipass');
// const baseCurrency = document.getElementById('basecurrency');
// const crypto = document.getElementById('crypto');
// const appSelection = document.getElementById('AppSelection');

// DOM Elements
const currentTime = document.querySelector("h1"),
  currentDateTime = document.querySelector("h2"),
  selectMenu = document.querySelectorAll("select"),
  appSelections = document.querySelectorAll("input[name='app']"),
  wifiSsid = document.querySelector('input[name="wifissid"]'),
  wifiPass = document.querySelector('input[name="wifipass"]'),
  // Buttons
  wifiSetBtn = document.querySelector('button.btn_SetWiFi'),
  cryptoSetBtn = document.querySelector('button.btn_SetCrypto'),
  appSelectBtn = document.querySelector('button.btn_SetApp'),
  clockSetBtn = document.querySelector('button.btn_SetClock');

const CurrencyUnitsList = ["usd", "eur", "gbp", "sats", "bitcoin"];
const CryptoUnitsList = ["cardano", "baked-token", "bitcoin", "dogecoin", "ethereum", "litecoin", "polkadot", "kusama"];
const DayOfWeekList = ["Sun", "Mon", "Tue", "Wed", "Thur", "Fri", "Sat"];
const MonthOfYearList = ["Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"];

let h, m, dow, d, mo, y;

for (let i = CurrencyUnitsList.length - 1; i >= 0; i--) {
  let option = `<option value="${CurrencyUnitsList[i]}">${CurrencyUnitsList[i]}</option>`;
  selectMenu[0].firstElementChild.insertAdjacentHTML("afterend", option);
}

for (let i = CryptoUnitsList.length - 1; i >= 0; i--) {
  let option = `<option value="${CryptoUnitsList[i]}">${CryptoUnitsList[i]}</option>`;
  selectMenu[1].firstElementChild.insertAdjacentHTML("afterend", option);
}

setInterval(() => {
  let date = new Date();
  h = date.getHours(),
    m = date.getMinutes(),
    s = date.getSeconds(),
    mo = date.getMonth() + 1,
    d = date.getDate(),
    y = date.getFullYear();
  ampm = "AM";
  if (h >= 12) {
    h = h - 12;
    ampm = "PM";
  }
  h = h == 0 ? h = 12 : h;
  h = h < 10 ? "0" + h : h;
  m = m < 10 ? "0" + m : m;
  s = s < 10 ? "0" + s : s;
  currentTime.innerText = `${h}:${m}:${s} ${ampm}`;
});

function getFirstLoad() {
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      var myObj = JSON.parse(this.responseText);
      console.log(myObj);
      let ssid = parseInt(myObj.ssid);
    }
  };
  xhr.open("GET", "/ReadingsFirstConfig", true);
  xhr.send();
}

function setWiFi() {
  let ssid = wifiSsid.value,
    pass = wifiPass.value;
  if (ssid == "" || pass == "") {
    return alert("Please, fill in the WiFi SSID and Password to set WiFi!");
  }
  let xhr = new XMLHttpRequest();
  let currentString = "/update?";
  currentString += "ssid=" + String(ssid) + "&pass=" + String(pass);
  xhr.open("GET", currentString, true);
  xhr.send();
  alert("Set WiFi done!");
}

function setRTCClock() {
  let date = new Date(),
    setHour = date.getHours(),
    setMin = date.getMinutes(),
    setSec = date.getSeconds(),
    setDayOfWeek = date.getDay(),
    setDate = date.getDate(),
    setMonth = date.getMonth() + 1,
    setYear = date.getFullYear();

  let xhr = new XMLHttpRequest();
  let currentString = "/update?";
  currentString += "hour=" + String(setHour) + "&min=" + String(setMin) + "&sec=" + String(setSec) + "&dayofweek=" + String(setDayOfWeek) + "&date=" + String(setDate) + "&month=" + String(setMonth) + "&year=" + String(setYear);
  xhr.open("GET", currentString, true);
  xhr.send();
  alert("Set time done!");
}

function setApp() {
  let _app = 0;
  let checkedApp = false;
  for (const app of appSelections) {
    if (app.checked) {
      _app = app.value;
      checkedApp = true;
      break;
    }
  }
  if (!checkedApp) {
    return alert("Please, select an app to set!");
  }
  let xhr = new XMLHttpRequest();
  let currentString = "/update?";
  currentString += "app=" + String(_app);
  xhr.open("GET", currentString, true);
  xhr.send();
  alert("Set app done!");
}

function setCrypto() {
  if (selectMenu[1].value == "Crypto" || selectMenu[0].value == "Currency") {
    return alert("Please, select a valid crypto and currency to set crypto!");
  }
  let crypto = selectMenu[1].value,
    currency = selectMenu[0].value;
  let xhr = new XMLHttpRequest();
  let currentString = "/update?";
  currentString += "crypto=" + String(crypto) + "&currency=" + String(currency);
  xhr.open("GET", currentString, true);
  xhr.send();
  alert("Set crypto done!");
}

if (!!window.EventSource) {
  var source = new EventSource('/events');
  source.addEventListener('new_readings', function (e) {
    console.log("new_readings", e.data);
    var myObj = JSON.parse(e.data);
    console.log(myObj);
    h = parseInt(myObj.hour),
      m = parseInt(myObj.min),
      dow = DayOfWeekList[parseInt(myObj.dow)],
      d = parseInt(myObj.date),
      mo = parseInt(myObj.mon),
      y = parseInt(myObj.year);
    // Add zero if 1 digit number
    h = h < 10 ? "0" + h : h,
      m = m < 10 ? "0" + m : m,
      d = d < 10 ? "0" + d : d,
      mo = mo < 10 ? "0" + mo : mo;
  }, false);
}

window.addEventListener('load', getFirstLoad);
wifiSetBtn.addEventListener("click", setWiFi);
cryptoSetBtn.addEventListener("click", setCrypto);
clockSetBtn.addEventListener("click", setRTCClock);
appSelectBtn.addEventListener("click", setApp);