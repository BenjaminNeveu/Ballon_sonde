function envoiTrame() {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "envoiTrame", true);
    xhr.send();
}

function recupererDonnee() {
    $.ajax({
        url: 'recupererDonnee',
        type: 'POST',
        dataType: 'json',
        success:
                function (donnees, status, xhr) {
                    $.each(donnees, function (index, ligne) {
                        $("#longitude").text("-longitude : " + ligne.longitude);
                        $("#latitude").text("-latitude : " + ligne.latitude);
                        $("#altitude").text("-altitude : " + ligne.altitude);
                        $("#radiation").text("-radiation : " + ligne.radiation);
                        $("#temperature").text("-temperature : " + ligne.temperature);
                        $("#pression").text("-pression : " + ligne.pression);
                        $("#humidite").text("-humidite : " + ligne.humidite);
                    });
                },
        error:
                function (xhr, status, error) {
                    console.log("param : " + JSON.stringify(xhr));
                    console.log("status : " + text);
                    console.log("error : " + error);
                }

    });
}

setInterval(function getData() {
    recupererDonnee();
}, 5000);