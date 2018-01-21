var currencies = ["XXBTZEUR","XETHZEUR","XXMRZEUR","XLTCZEUR","BCHEUR","XREPZEUR", "DASHEUR"];
var currencyNames = ["Bitcoin","Ethereum","Monero","Litecoin","Bitcoin cash","Augur","Dash"];
//var currencyNames = ["Bitcoin (BTC)","Ethereum (ETH)","Monero (XMR)","Litecoin (LTC)","Bitcoin cash (BCH)","Augur (REP)"];

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log('PebbleKit JS ready!');
    processBTCValue();
  }
);

function httpGetASync(theUrl, fn) {

  var xhr = new XMLHttpRequest();
  xhr.open("GET", theUrl, true);
  xhr.onload = function (e) {
    if (xhr.readyState === 4) {
      if (xhr.status === 200) {
        fn(xhr.responseText);
      } else {
        console.error(xhr.statusText);
      }
    }
  };
  xhr.onerror = function (e) {
    console.error(xhr.statusText);
  };
  xhr.send(null);
}

function formatNumber(f) {
	return Math.round(f).toString().replace(/\B(?=(\d{3})+(?!\d))/g, ",");
}

function getBTCValueFromKrakenMessage(json) {
  var msg = JSON.parse(json);

  try {
    var currencyValue = [];
    
    for(index in currencies) {
      
      var node = msg.result[currencies[index]]
      
      var a = parseFloat(node.a[0]);
      var b = parseFloat(node.b[0]);
          
      var value = formatNumber((a + b) / 2.0);
      currencyValue.push(value);
    }
    
    return { 
      'N': currencyNames.join("\n"),
      'V': currencyValue.join("\n"),
    };
  } catch(error) {
    console.log(error);
    return { 
      'N': "ERR",
      'V': error
    };
  }
}

function sendBTCValueToPebble(values) {

      // Send to Pebble
    Pebble.sendAppMessage(values); //,
}

function processBTCValue() {

    var krakenUrl = "https://api.kraken.com/0/public/Ticker?pair=" + currencies.join(",");
    
    httpGetASync(krakenUrl, function(response)
    {
      var v = getBTCValueFromKrakenMessage(response);
      sendBTCValueToPebble(v);
      //console.log("BTC: " + v.B2);
      //console.log("XMR: " + v.M2);
    });
}

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    processBTCValue();
  }                     
);