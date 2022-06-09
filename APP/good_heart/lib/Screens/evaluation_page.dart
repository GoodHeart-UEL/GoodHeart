import 'dart:convert';
import 'package:flutter/material.dart';
import 'package:good_heart/Components/alert_dialog.dart';
import 'package:good_heart/Components/display_files.dart';
import 'package:good_heart/Components/show_evaluation.dart';
import 'package:good_heart/Services/communication_with_server.dart';
import 'package:good_heart/Screens/connection_page.dart';
import 'package:good_heart/Services/json_to_list.dart';
import '../main.dart';
import '../globals.dart' as globals;
import 'package:logger/logger.dart';

var logger = Logger(
  filter: null,
  printer: PrettyPrinter(),
  output: null,
);

var LoggerClass = "EvaluationPage";

class EvaluationPage extends StatefulWidget {
  Wrapper? socket;
  EvaluationPage({Key? key, this.socket}) : super(key: key);

  @override
  _EvaluationState createState() => _EvaluationState(this.socket);
}

class _EvaluationState extends State<EvaluationPage> {
  Wrapper? socket;

  var _listOfFiles = [];
  CommunicationWithServer _serverEval = CommunicationWithServer();


  _EvaluationState(Wrapper? socket){
    this.socket = socket;
    if(globals.isConnected == 1){
      startSocketListenInEvaluationPage();
    }
  }

  void startSocketListenInEvaluationPage(){
    socket!.listener.listen((List<int> bytes) async {
        logger.d("[$LoggerClass] Bytes list of files: $bytes");

        var stringBytesReceived = new String.fromCharCodes(bytes).trim();
        logger.d("[$LoggerClass] Getting from server: $stringBytesReceived");

        var receivedFromServer = CommunicationWithServer.fromJson(jsonDecode(stringBytesReceived));

        switch(receivedFromServer.OpCode){
          case 610:
            globals.chosenFileName = null;
            var listOfFiles = jsonToList(stringBytesReceived);
            setState(() {
              _listOfFiles = listOfFiles;
              logger.d("[$LoggerClass] Ecg list of files from json: $_listOfFiles");
            });
            //await askForFile(context);
            await DisplayFiles(context, _listOfFiles);

            // send to server in Json format
            if (globals.chosenFileName != null) {
              logger.d("[$LoggerClass] chosenFileName: ${globals.chosenFileName}");
              globals.idMsgValue += 1;
              var fileChoice = CommunicationWithServer(IdMsg: globals.idMsgValue, OpCode: 100, ECGFile: globals.chosenFileName);
              socket!.client!.write(fileChoice.toJson());
            }
            break;

          case 400:
            setState(() {
              _serverEval = receivedFromServer;
            });
            await showServerEvaluation(context, _serverEval);
            break;

          default:
            logger.d("[$LoggerClass] Sent from server unknown OpCode");
            break;
        }
      },

      onError: (error, StackTrace trace) async {
        logger.e("[$LoggerClass] Error retriving ecg files: $error\n$trace");
      },

      cancelOnError: false

    );
  }

  @override
  Widget build(BuildContext context) {
    var screenSize = MediaQuery.of(context).size;
    return Scaffold(
      appBar: AppBar(
        title: const Text('Evaluation'),
      ),
      body: SafeArea(
              child: ListView(
                  children: <Widget>[
                    Card(
                      child: Padding(
                        padding: EdgeInsets.only(top: screenSize.height * 0.01),
                      ),
                      shadowColor: const Color.fromRGBO(255, 255, 255, 0),
                      color: Colors.grey[50],
                      margin: EdgeInsets.only(left:20, right:20, top: 10),
                    ),
                        Card(
                            child: ListTile(
                              leading: Icon(Icons.add_circle, size: 40,),
                              title: Text('Capture new ECG', style: TextStyle(height: 1.4, fontSize: 20),),
                              subtitle: Text('Capture new ECG by placing sensors all over patients chest.', style: TextStyle(height: 1.3),),
                              isThreeLine: true,
                              onTap: () async {
                                await alertDialog(
                                  context, 
                                  Text("Hold on!\nUnfortunately this feature is not yet implemented..."),
                                  Text("Maybe next time."),
                                );
                              },
                          ),
                          margin: EdgeInsets.only(left:20, right:20, top: 10),
                        ),
                        Card(
                          child: ListTile(
                            leading: FittedBox(
                              child: Icon(Icons.folder, size: 40,),
                              fit: BoxFit.fitHeight,
                            ),
                            title: Text('Find an ECG file', style: TextStyle(height: 1.4, fontSize: 20),),
                            subtitle: Text('Find a previously generated file with ECG info.', style: TextStyle(height: 1.3),),
                            isThreeLine: true,
                            onTap: () async {
                              try {
                                globals.idMsgValue += 1;
                                var sendToServer = CommunicationWithServer(IdMsg: globals.idMsgValue, OpCode: 600);
                                var sendToServerJson = sendToServer.toJson();
                                socket!.client!.write(sendToServerJson);
                                logger.d("[$LoggerClass] Sending ecg file request to server: $sendToServerJson");

                              }catch(_) {                                
                                await alertDialog(
                                  context, 
                                  Text("You are not connected to a host."),
                                  Text("Connect now."),
                                  );
                                  Navigator.pushReplacement(context, MaterialPageRoute(builder: (context) => ConnectionPage(socket: socket,)));
                              }
                            },
                          ),
                          margin: EdgeInsets.only(left:20, right:20, top: 10),
                        ),
                      ],
              ),
            ),
    );
  }
}