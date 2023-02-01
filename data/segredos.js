let pegaInformacoes = () => {
    let infos = {
        "clientId": document.getElementById('nome_cliente').value,
        "secret": document.getElementById('segredo').value
    };


    fetch('/setsecrets', {
            method: 'POST', // or 'PUT'
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(infos),
        })
        .then((response) => response.json())
        .then((data) => {
            console.log('Success:', data);
            location.href = '/';
        })
        .catch((error) => {
            console.error('Error:', error);
            document.getElementById('body').classList.toggle('loading');
        });
}

document.getElementById('btnenviar').onclick = () => {
    document.getElementById('body').classList.toggle('loading');
    pegaInformacoes();
}

let pegaStreamersJaCadastrados = () => {

    fetch('/secrets.json')
        .then((response) => response.json())
        .then((data) => {
            if (data !== undefined) {
                document.getElementById('nome_cliente').value = data.clientId;
                document.getElementById('segredo').value = data.secret;
            } else {
                console.log("Nao consegui recuperar informaçoes");
            }
            document.getElementById('body').classList.toggle('loading');
        }).catch(error => {
            document.getElementById('body').classList.toggle('loading');
        });


}

pegaStreamersJaCadastrados();

function hidePW() {
    var x = document.getElementById("segredo");
    if (x.type === "password") {
        x.type = "text";
    } else {
        x.type = "password";
    }
}

document.getElementById('mostrar').onclick = () => {
    hidePW();
}