module MyClient = Client.Make(SocketMessages);

let socket = MyClient.create();

MyClient.emit(socket, Hi);

ReactDOMRe.renderToElementWithId(<App />, "game");
