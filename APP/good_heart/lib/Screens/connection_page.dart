import 'dart:async';
import 'dart:convert';
import 'dart:io';
import 'package:flutter/material.dart';
import 'package:good_heart/Theme/colors.dart';
import 'package:good_heart/main.dart';
import 'package:animated_text_kit/animated_text_kit.dart';
import 'package:good_heart/globals.dart' as globals;
import 'package:good_heart/Services/communication_with_server.dart';
import 'package:logger/logger.dart';

var logger = Logger(
  filter: null,
  printer: PrettyPrinter(),
  output: null,
);

var LoggerClass = "ConnectionPage";

class ConnectionPage extends StatefulWidget {

  Wrapper? socket;
  ConnectionPage({Key? key, this.socket}) : super(key: key);

  @override
  _ConnectionPage createState() => _ConnectionPage(this.socket);
}

class _ConnectionPage extends State<ConnectionPage> {

  final GlobalKey<FormState> _formKey = GlobalKey<FormState>();
  final TextEditingController _textEditingControllerIP = TextEditingController();
  final TextEditingController _textEditingControllerConnectionTest = TextEditingController();
  final TextEditingController _textForAppearText = TextEditingController();

  Wrapper? socket;
  int _pressedOkInConnect = 0;

  _ConnectionPage(Wrapper? socket){
      this.socket = socket;
    if(globals.isConnected == 1){
      startSocketListenInConnectionPage();
    }
  }

  void startSocketListenInConnectionPage(){
    socket!.listener.listen((List<int> bytes) async { // AQUI acho que não tem o await
      var receivedFromServer = CommunicationWithServer.fromJson(jsonDecode(new String.fromCharCodes(bytes).trim()));

      _textEditingControllerConnectionTest.text = "Not connected";

      switch(receivedFromServer.OpCode){
        case 510:
          setState(() {
            _textEditingControllerConnectionTest.text = "Connected";
          });
          await showAlertServerAnswer(context);
          break;
        default:
          logger.d("[$LoggerClass] Sent from server unknown OpCode");
          break;

      }
    },

    onError: (error, StackTrace trace) async {
      await showAlertErrorSocket(context);
    },

    cancelOnError: false
    );
  }

  // Dialog Structure
  Future<void> showIPDialog(BuildContext context) async {
    return await showDialog(context: context,
        builder: (context){
          bool isChecked = false;
          return StatefulBuilder(builder: (context,setState){
            return AlertDialog(
              content: Form(
                  key: _formKey,
                  child: Column(
                    mainAxisSize: MainAxisSize.min,
                    children: [
                      Text("Enter your host IP: ", style: TextStyle(height: 1.2, fontSize: 20),),
                      TextFormField(
                      controller: _textEditingControllerIP,
                        validator: (value){
                          return value!.isNotEmpty ? null : "Invalid Field";
                        },
                        decoration: InputDecoration(hintText: "192.168.0.101"),
                      ),
                    ],
                  )
              ),
              actions: <Widget>[
                TextButton(
                  child: Text('Ok'),
                  onPressed: (){
                    if(_formKey.currentState!.validate()){ // AQUI
                      _pressedOkInConnect = 1;
                      // Do something like updating SharedPreferences or User Settings etc.
                      Navigator.of(context).pop();
                    }
                  },
                ),
              ],
            );
          });
        });
  }

  Future<void> getIPDialog(BuildContext context) async {
    return await showDialog(context: context,
        builder: (context){
          return AlertDialog(
            content: Text("IP: " + _textEditingControllerIP.text, style: TextStyle(height: 1.2, fontSize: 18),),
            actions: <Widget> [
              TextButton(
                  onPressed: (){
                    Navigator.of(context).pop();
                  },
                  child: Text("Ok"),
              )
            ]
          );
    });
  }

  Future<void> showAlertUnableToConnect(BuildContext context) async {
    return await showDialog(context: context,
        builder: (context){
          return AlertDialog(
            content: Text("\"" + _textEditingControllerIP.text + "\" \ndoesn't seem to be a valid IP.", style: TextStyle(fontSize: 18),),
            actions: <Widget> [
              TextButton(
                  onPressed: (){
                    Navigator.of(context).pop();
                  },
                  child: Text("Ok"),
              )
            ]
          );
    });
  }

  Future<void> showAlertServerAnswer(BuildContext context) async {
    return await showDialog(context: context,
        builder: (context){
          return AlertDialog(
            content: _checkingConnectionText(_textEditingControllerConnectionTest),

            actions: <Widget> [
              TextButton(
                  onPressed: (){
                    Navigator.of(context).pop();
                  },
                  child: Text("Ok"),
              )
            ]
          );
    });
  }

  Future<void> showAlertErrorSocket(BuildContext context) async {
    return await showDialog(context: context,
        builder: (context){
          return AlertDialog(
            content: Text("Socket ERROR."),
            actions: <Widget> [
              TextButton(
                  onPressed: (){
                    Navigator.of(context).pop();
                  },
                  child: Text("Ok"),
              )
            ]
          );
    });
  }

  Future<void> showAlertPassedSocket(BuildContext context) async {
    return await showDialog(context: context,
        builder: (context){
          return AlertDialog(
              content: _appearText(_textForAppearText),
              actions: <Widget> [
                TextButton(
                  onPressed: (){
                    Navigator.of(context).pop();
                  },
                  child: Text("Ok"),
                )
              ]
          );
        });
  }

  // Connection Page Structure
  @override
  Widget build(BuildContext context) {

    var screenSize = MediaQuery.of(context).size;

    return Scaffold(
      appBar: AppBar(
        title: const Text('Device connection'),
      ),
      body: SafeArea(
        child: ListView(
            children: <Widget>[
              Card( //AQUI não sei se esse align faz sentido dentro de um listview, já que ele não faz nada
                child: Image.asset(
                  "assets/images/Gifs/HeartBeatBranco.gif",
                  height: 300.0,
                  width: 300.0,
                ),
                shadowColor: const Color.fromRGBO(255, 255, 255, 0),
                color: Colors.grey[50],
                margin: EdgeInsets.all(20),
              ),
              Card(
                child: ListTile(
                  leading: Icon(Icons.mobile_friendly_rounded,  size: 40,),
                  title: Text("Connect to Device", style: TextStyle(height: 1, fontSize: 20),),
                  //subtitle: Text('Here is a second line'),
                  tileColor: MyColors.green[800], //AQUI
                  shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(10.0),
                  ),
                  onTap: () async {
                    await showIPDialog(context);
                    try {
                      if(_pressedOkInConnect != 0) {
                        socket!.setClient(await Socket.connect(
                            _textEditingControllerIP.text, 3333));
                        _pressedOkInConnect = 0;
                        // Resets the idMsgValue
                        globals.idMsgValue = 0;
                        startSocketListenInConnectionPage();
                        globals.isConnected = 1;
                      }
                      } catch(_) {
                      if(_pressedOkInConnect != 0) {
                        await showAlertUnableToConnect(context);
                        _pressedOkInConnect = 0;
                      }
                    }

                  },
                ),
                shadowColor: const Color.fromRGBO(255, 255, 255, 0),
                margin: EdgeInsets.only(left:20, right:20, top: 10),
              ),
              Card(
                child: ListTile(
                  leading: Icon(Icons.mobile_off_rounded,  size: 40,),
                  title: Text("Disconnect from Device", style: TextStyle(height: 1, fontSize: 20),),
                  //subtitle: Text('Here is a second line'),
                  tileColor: MyColors.green[800], //AQUI
                  shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(10.0),
                  ),
                  onTap: () async {
                    try {
                      var sendToServer = CommunicationWithServer(IdMsg: globals.idMsgValue, OpCode: 700);
                      socket!.client!.write(sendToServer.toJson());
                      socket!.client!.close();
                      _textForAppearText.text = "Disconnected";
                      await showAlertPassedSocket(context);
                      globals.isConnected = 0;

                    }catch(_) { // AQUI acho que tá com um bug
                      _textForAppearText.text = "Disconnected";
                      await showAlertPassedSocket(context);
                      //await showAlertErrorSocket(context);
                    }
                  },
                ),
                shadowColor: const Color.fromRGBO(255, 255, 255, 0),
                margin: EdgeInsets.only(left:20, right:20, top: 10),
              ),
              Card(
                child: ListTile(
                  leading: Icon(Icons.cable,  size: 40,),
                  title: Text("Check connection", style: TextStyle(height: 1, fontSize: 20),),
                  //subtitle: Text('Here is a second line'),
                  tileColor: MyColors.green[800], //AQUI
                  shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(10.0),
                  ),
                  onTap: () async {
                    try {
                      globals.idMsgValue += 1;
                      var sendToServer = CommunicationWithServer(IdMsg: globals.idMsgValue, OpCode: 500);
                      socket!.client!.write(sendToServer.toJson());

                    }catch(_) {
                      _textEditingControllerConnectionTest.text = "Not connected";
                      await showAlertServerAnswer(context);
                      print(_);
                    }

                  },
                ),
                shadowColor: const Color.fromRGBO(255, 255, 255, 0),
                margin: EdgeInsets.only(left:20, right:20, top: 10),
              ),
              Card(
                child: ListTile(
                  leading: Icon(Icons.scatter_plot,  size: 40,),
                  title: Text("Show IP", style: TextStyle(height: 1, fontSize: 20),),
                  //subtitle: Text('Here is a second line'),
                  tileColor: MyColors.green[800], //AQUI
                  shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(10.0),
                  ),
                  onTap: () async {
                    await getIPDialog(context);
                  },
                ),
                shadowColor: const Color.fromRGBO(255, 255, 255, 0),
                margin: EdgeInsets.only(left:20, right:20, top: 10),
              ),
            ]
        ),

      ),

    );
  }
}

Widget _appearText(_text){

  const colorizeTextStyle = TextStyle(
    fontSize: 18.0,
    fontFamily: 'Horizon',
  );

  return SizedBox(
    width: 250.0,
    child: DefaultTextStyle(
      style: const TextStyle(
        fontSize: 18.0,
        fontFamily: 'popin',
        color: Color.fromRGBO(0, 0, 0, 1),
      ),
      child: AnimatedTextKit(
          isRepeatingAnimation: false,
          animatedTexts: [
            ColorizeAnimatedText("\"" + _text.text + "\".",
              textStyle: colorizeTextStyle,
              colors: [Color.fromRGBO(0, 0, 0, 1), Color.fromRGBO(0, 0, 0, 1),]),
          ]
      ),
    ),
  );
}


Widget _checkingConnectionText(_text){

  const colorizeTextStyle = TextStyle(
    fontSize: 18.0,
    fontFamily: 'Horizon',
  );

  return SizedBox(
    width: 250.0,
    child: DefaultTextStyle(
      style: const TextStyle(
        fontSize: 18.0,
        fontFamily: 'popin',
        color: Color.fromRGBO(0, 0, 0, 1),
      ),
      child: AnimatedTextKit(
          isRepeatingAnimation: false,
          animatedTexts: [
            TyperAnimatedText('Checking...'
                ,speed: Duration(milliseconds: 100)),
            ColorizeAnimatedText("\"" + _text.text + "\".",
                textStyle: colorizeTextStyle,
                colors: [Color.fromRGBO(0, 0, 0, 1), Color.fromRGBO(0, 0, 0, 1),]),
        ],
      ),
    ),
  );
}