let pegaCor = (valor, cor) => {
  let retorno = valor.replace("rgb(","");
  retorno = retorno.replace(")","");

  switch (cor) {
    case "R":
      retorno = retorno.split(",")[0]
      break;
    case "G":
      retorno = retorno.split(",")[1]
      break;
    case "B":
      retorno = retorno.split(",")[2]
      break;
  }

  return retorno;
}

let pegaInformacoes = () => {
  let streamers = [];

  for (let index = 1; index <= 5; index++) {
    let streamer = 'streamer_name' + index;
    let cor = 'cor' + index;

    let item = {
      "streamer": document.getElementById(streamer).value,
      "R": pegaCor(document.getElementById(cor).value, "R"),
      "G": pegaCor(document.getElementById(cor).value, "G"),
      "B": pegaCor(document.getElementById(cor).value, "B")
    };
    streamers.push(item);
  }

  fetch('/setstreamer', {
      method: 'POST', // or 'PUT'
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify(streamers),
    })
    .then((response) => response.json())
    .then((data) => {
      console.log('Success:', data);
      location.href = '/';
    })
    .catch((error) => {
      console.error('Error:', error);
    });
}

document.getElementById('btnenviar').onclick = () => {
  pegaInformacoes();
}

let pegaStreamersJaCadastrados = () => {

  fetch('/streamers.json')
    .then((response) => response.json())
    .then((data) => {
      if(data !== undefined) {
        let contador = 1;
        data.forEach(element => {
          let streamer = 'streamer_name'+contador;
          let cor = 'cor'+contador;
          document.getElementById(streamer).value = element.streamer;
          document.getElementById(cor).jscolor.fromString('rgb(' + element.R + ',' + element.G + ',' + element.B + ')');
          contador++;
        });
      } else {
        console.log("Nao consegui recuperar informaçoes");
      }
      document.getElementById('body').classList.toggle('loading');
    }).catch(error => {
      document.getElementById('body').classList.toggle('loading');
    });
}

pegaStreamersJaCadastrados();