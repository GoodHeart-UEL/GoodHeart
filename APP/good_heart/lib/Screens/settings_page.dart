import 'package:flutter/material.dart';
import 'package:good_heart/Components/info_dialog.dart';
import 'package:good_heart/Theme/colors.dart';
import 'package:good_heart/main.dart';
import 'dart:io';


class SettingsPage extends StatefulWidget {

  Wrapper? socket;
  SettingsPage({Key? key, this.socket}) : super(key: key);

  @override
  _SettingsPageState createState() => _SettingsPageState(client: this.socket!.client);
}

class _SettingsPageState extends State<SettingsPage> {

  Socket? client;
  _SettingsPageState({this.client});

  @override
  Widget build(BuildContext context) {
    var screenSize = MediaQuery.of(context).size;
    return Scaffold(
      appBar: AppBar(
        title: const Text('Settings'),
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
                  leading: Icon(Icons.info),
                  title: Text('Info', style: TextStyle(fontSize: 18),),
                  dense: true,
                  onTap: () async {
                    await infoDialog(
                      context, 
                      Text("Version: 1.0 \nDate: 07-10-2021\n", style: TextStyle(height: 1.2, fontSize: 18),),
                      Text("Ok"),
                      Icon(Icons.medical_services_rounded, color: MyColors.red, size: 50.0,),
                      );
                  },
                ),
                margin: EdgeInsets.only(left:20, right:20, top: 10),
              ),
            ],
          )
      ),
    );
  }
}