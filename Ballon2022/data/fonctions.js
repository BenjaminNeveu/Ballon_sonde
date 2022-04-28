function envoiTrame() {
    $.ajax({
        url: "/envoiTrame",
        type: 'GET',
        success:
                function (donnees, status, xhr)
                {
                    console.log(donnees);
                },
        error:
                function (xhr, status, error)
                {
                    console.log("param : " + JSON.stringify(xhr));
                    console.log("status : " + status);
                    console.log("error : " + error);
                }
    });
}

function desactiver() {

    let message = "Êtes-vous sûr ?";

    if (confirm(message) === true) {
        $.ajax({
            url: "/desactiverWifi",
            type: 'GET',
            success:
                    function (donnees, status, xhr)
                    {
                        console.log(donnees);
                    },
            error:
                    function (xhr, status, error)
                    {
                        console.log("param : " + JSON.stringify(xhr));
                        console.log("status : " + status);
                        console.log("error : " + error);
                    }
        });
    } 
}


function recupererDonnee() {
    $.getJSON("/recupererDonnee")
            .done(function (resultat, status, xhr) {

                console.log(resultat);

                $("#longitude").text(resultat.longitude);
                $("#latitude").text(resultat.latitude);
                $("#altitude").text(resultat.altitude);
                $("#radiation").text(resultat.radiation);
                $("#temperature").text(resultat.temperature);
                $("#humidite").text(resultat.humidite);
                $("#pression").text(resultat.pression);
            })
            .fail(function (xhr, status, error) {
                console.log(xhr);
                console.log(status);
                console.log(error);
            });
}

setInterval(function getData() {
    recupererDonnee();
}, 30000);

