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

const CurrencyUnitsList = ["USD", "EUR", "GBP", "SATOSHI", "BTC"];
const CryptoUnitsList = ["Cardano", "Baked Token", "Bitcoin", "Dogecoin", "Ethereum", "Litecoin", "Polkadot", "Kusama"];
const DayOfWeekList = ["Sun","Mon", "Tue", "Wed","Thur","Fri","Sat"];
const MonthOfYearList = ["Jan","Feb", "Mar", "Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"];

let h,m,dow,d,mo,y;

// for (let i = 12; i > 0; i--) {
//     i = i < 10 ? `0${i}` : i;
//     let option = `<option value="${i}">${i}</option>`;
//     selectMenu[0].firstElementChild.insertAdjacentHTML("afterend", option);
// }

// for (let i = 59; i >= 0; i--) {
//     i = i < 10 ? `0${i}` : i;
//     let option = `<option value="${i}">${i}</option>`;
//     selectMenu[1].firstElementChild.insertAdjacentHTML("afterend", option);
// }

// for (let i = 2; i > 0; i--) {
//     let ampm = i == 1 ? "AM" : "PM";
//     let option = `<option value="${ampm}">${ampm}</option>`;
//     selectMenu[2].firstElementChild.insertAdjacentHTML("afterend", option);
// }

for(let i = CurrencyUnitsList.length - 1; i >= 0; i--) {
    let option = `<option value="${CurrencyUnitsList[i]}">${CurrencyUnitsList[i]}</option>`;
    selectMenu[0].firstElementChild.insertAdjacentHTML("afterend", option);
}

for(let i = CryptoUnitsList.length - 1; i >= 0; i--) {
    let option = `<option value="${CryptoUnitsList[i]}">${CryptoUnitsList[i]}</option>`;
    selectMenu[1].firstElementChild.insertAdjacentHTML("afterend", option);
}

for (let i = 23; i >= 0; i--) {
    i = i < 10 ? `0${i}` : i;
    let option = `<option value="${i}">${i}</option>`;
    selectMenu[2].firstElementChild.insertAdjacentHTML("afterend", option);
}

for (let i = 59; i >= 0; i--) {
    i = i < 10 ? `0${i}` : i;
    let option = `<option value="${i}">${i}</option>`;
    selectMenu[3].firstElementChild.insertAdjacentHTML("afterend", option);
}

for (let i = DayOfWeekList.length - 1; i >= 0; i--) {
    let option = `<option value="${i}">${DayOfWeekList[i]}</option>`;
    selectMenu[4].firstElementChild.insertAdjacentHTML("afterend", option);
}

for (let i = 31; i > 0; i--) {
    i = i < 10 ? `0${i}` : i;
    let option = `<option value="${i}">${i}</option>`;
    selectMenu[5].firstElementChild.insertAdjacentHTML("afterend", option);
}

for (let i = MonthOfYearList.length - 1; i >= 0; i--) {
    let option = `<option value="${i}">${MonthOfYearList[i]}</option>`;
    selectMenu[6].firstElementChild.insertAdjacentHTML("afterend", option);
}

for (let i = 2099; i >= 2024; i--) {
    let option = `<option value="${i}">${i}</option>`;
    selectMenu[7].firstElementChild.insertAdjacentHTML("afterend", option);
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
    if(h >= 12) {
        h = h - 12;
        ampm = "PM";
    }
    h = h == 0 ? h = 12 : h;
    h = h < 10 ? "0" + h : h;
    m = m < 10 ? "0" + m : m;
    s = s < 10 ? "0" + s : s;
    currentTime.innerText = `${h}:${m}:${s} ${ampm}`;
    /*==================================================*/
    // if(h == null) {
    //   h = 0;
    //   h = "0" + h;
    // }
    // if(m == null) {
    //   m = 0;
    //   m = "0" + m;
    // }
    // if(dow == null) {
    //   dow = DayOfWeekList[0];
    // }
    // if(d == null) {
    //   d = 0;
    //   d = "0" + d;
    // }
    // if(mo == null) {
    //   mo = 0;
    //   mo = "0" + mo;
    // }
    // if(y == null) {
    //   y = 0;
    //   y = "000" + y;
    // }
    // currentTime.innerText = `${dow} ${h}:${m}`;
    currentDateTime.innerText = `${d}-${mo}-${y}`;
});

function getFirstLoad() {
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var myObj = JSON.parse(this.responseText);
      console.log(myObj);
      let h = parseInt(myObj.hour),
      m = parseInt(myObj.min),
      dow = DayOfWeekList[parseInt(myObj.dow)],
      d = parseInt(myObj.date),
      mo = parseInt(myObj.mon),
      y = parseInt(myObj.year);

      h = h < 10 ? "0" + h : h;
      m = m < 10 ? "0" + m : m;
      d = d < 10 ? "0" + d : d;
      mo = mo < 10 ? "0" + mo : mo;
    }
  };
  xhr.open("GET", "/ReadingsFirstConfig", true);
  xhr.send();
}

function setWiFi() {
    let ssid = wifiSsid.value,
    pass = wifiPass.value;
    if(ssid == "" || pass == "") {
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
//   let time = `${selectMenu[5].value}:${selectMenu[6].value}:${selectMenu[7].value}:${selectMenu[8].value}:${selectMenu[9].value}:${selectMenu[10].value}`;
//   if (time.includes("Hour") || time.includes("Minute") || time.includes("DayOfWeek") || time.includes("Date") || time.includes("Month") || time.includes("Year")) {
//     return alert("Please, select a valid time to set clock!");
//   }

//   let setHour = selectMenu[5].value,
//   setMin = selectMenu[6].value,
//   setDayOfWeek = selectMenu[7].value,
//   setDate = selectMenu[8].value,
//   setMonth = selectMenu[9].value,
//   setYear = selectMenu[10].value;
    let date = new Date(),
    setHour = date.getHours(),
    setMin = date.getMinutes(),
    setDayOfWeek = date.getDay(),
    setDate = date.getDate(),
    setMonth = date.getMonth() + 1,
    setYear = date.getFullYear();

  let xhr = new XMLHttpRequest();
  let currentString = "/update?";
  currentString += "hour=" + String(setHour) + "&min=" + String(setMin) + "&dayofweek=" + String(setDayOfWeek) + "&date=" + String(setDate) + "&month=" + String(setMonth) + "&year=" + String(setYear);
  xhr.open("GET", currentString, true);
  xhr.send();
  alert("Set time done!");
}

function setApp() {
    let _app = 0;
    let checkedApp = false;
    for(const app of appSelections) {
        if(app.checked) {
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
    if(selectMenu[4].value == "Crypto" || selectMenu[3].value == "Currency") {
        return alert("Please, select a valid crypto and currency to set crypto!");
    }
    let crypto = selectMenu[4].value,
    currency = selectMenu[3].value;
    let xhr = new XMLHttpRequest();
    let currentString = "/update?";
    currentString += "crypto=" + String(crypto) + "&currency=" + String(currency);
    xhr.open("GET", currentString, true);
    xhr.send();
    alert("Set crypto done!");
}

if (!!window.EventSource) {
  var source = new EventSource('/events');
  source.addEventListener('new_readings', function(e) {
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