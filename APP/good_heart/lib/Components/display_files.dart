import 'package:flutter/material.dart';

import '../globals.dart' as globals;

Future<void> DisplayFiles(BuildContext context, dynamic _listOfFiles) async {
    return await showDialog(context: context,
        builder: (context){
          return StatefulBuilder(
            builder: (context,setState){
            return AlertDialog(
                  content: Container (
                    height: 300.0, // Change as per your requirement
                    width: 300.0,
                    padding: EdgeInsets.only(top: 10),
                    child: ListView.builder(
                      itemCount: _listOfFiles.length,
                      itemBuilder: (context, index) {
                        return Card(
                          child: ListTile(
                            leading: Icon(Icons.description_rounded),
                            title: Text(_listOfFiles[index].toString(),
                                style: TextStyle(height: 1.2, fontSize: 18)),
                            dense: true,
                            onTap: () {
                              globals.chosenFileName = _listOfFiles[index].toString();
                              Navigator.of(context).pop();
                            },
                          ),
                        );
                      },
                    )
                  ),
              actions: <Widget>[
                TextButton(
                  child: Text('Back'),
                  onPressed: (){
                    globals.chosenFileName = null;
                    Navigator.of(context, rootNavigator: true).pop();
                  },
                ),
              ],
            );
          });
        }
      );
  }