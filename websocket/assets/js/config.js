let broker = {
    host: '149.56.36.129',
    port: 18083,
    clientId: 'web_' + parseInt(Math.random()*10000,10)
}

let client = new Paho.MQTT.Client(broker.host, Number(broker.port), broker.clientId)

client.onConnectionLost = onConnectionLost
client.onMessageArrived = onMessageArrived

let options = {
    onSuccess:onConnect
}

client.connect(options)            
function onConnect() {
    console.log("Koneksi berhasil!")
    document.getElementById("log").innerHTML += "Koneksi berhasil!<br>"
    client.subscribe("lampu")
    message = new Paho.MQTT.Message("Halo! saya " + broker.clientId)
    message.destinationName ="lampu"
    client.send(message)
}

function ledState(no, state) {
    if (state == 1) {
        message = new Paho.MQTT.Message("#on_" + no)
    }
    if (state == 0) {
        message = new Paho.MQTT.Message("#off_" + no)
    }
    message.destinationName ="lampu"
    client.send(message)
}

function custom() {
    let pesan = document.getElementById('pesan').value
    message = new Paho.MQTT.Message(pesan)
    message.destinationName = "lampu"
    client.send(message)
}

function onConnectionLost(responseObject) {
    if (responseObject.errorCode !== 0) {
        console.log("Koneksi gagal: "+responseObject.errorMessage)
        document.getElementById("log").innerHTML += "Koneksi gagal: "+responseObject.errorMessage+"<br>"
    }
}

function onMessageArrived(message) {
    console.log("Pesan baru: "+message.payloadString)
    document.getElementById("log").innerHTML += "Pesan baru: "+message.payloadString+"<br>"
}
