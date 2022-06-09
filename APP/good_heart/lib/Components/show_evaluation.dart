import 'package:flutter/material.dart';
import 'package:good_heart/Services/communication_with_server.dart';

Future<void> showServerEvaluation(BuildContext context, CommunicationWithServer _serverEval) async {
    return await showDialog(context: context,
        builder: (context){
          return AlertDialog(
            content: Text("Evaluation ready!", textAlign: TextAlign.center,
                style: TextStyle(height: 2.5, fontSize: 20)),
              actions: <Widget> [
                Card(
                  child: ListTile(
                    leading: Icon(Icons.medical_services_rounded),
                    title: Text("Heart rate: " + _serverEval.FreqCard.toString() + " bpm",
                        style: TextStyle(height: 0, fontSize: 18)),
                    dense: true,
                  ),
                  margin: EdgeInsets.only(left:20, right:20, top: 10),
                  elevation: 0.0,
                ),
                Card(
                  child: ListTile(
                    leading: Icon(Icons.sentiment_very_satisfied_rounded),
                    title: Text("Good complex: " + _serverEval.GoodComplex.toString(),
                        style: TextStyle(height: 0, fontSize: 18)),
                    dense: true,
                  ),
                  margin: EdgeInsets.only(left:20, right:20, top: 10),
                  elevation: 0.0,
                ),
                Card(
                  child: ListTile(
                    leading: Icon(Icons.sentiment_very_dissatisfied),
                    title: Text("Bad complex: " + _serverEval.BadComplex.toString(),
                        style: TextStyle(height: 0, fontSize: 18)),
                    dense: true,
                  ),
                  margin: EdgeInsets.only(left:20, right:20, top: 10),
                  elevation: 0.0,
                ),

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
