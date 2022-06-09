import 'package:flutter/material.dart';
import 'Theme/colors.dart';
import 'Screens/home_page.dart';
import 'Screens/connection_page.dart';
import 'Screens/evaluation_page.dart';
import 'Screens/settings_page.dart';
import 'dart:io';

void main() {
  runApp(MyApp());
}

class Wrapper {
  Socket? client;
  var listener;
  Wrapper();
  void setClient(Socket sock) {
    this.client = sock;
    this.listener = sock.asBroadcastStream();
  }
}

class MyApp extends StatelessWidget {

  var socket = new Wrapper();

  @override

  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      theme: ThemeData(
        primarySwatch: MyColors.green,
        visualDensity: VisualDensity.adaptivePlatformDensity,
      ),
      routes: {
        '/': (_) => DefaultHomePage(),
        '/connection_page': (_) => ConnectionPage(socket: socket),
        '/evaluation_page': (_) => EvaluationPage(socket: socket),
        '/settings_page': (_) => SettingsPage(socket: socket),
      }
    );
  }
}
