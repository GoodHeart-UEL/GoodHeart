import 'package:flutter/material.dart';
import 'package:good_heart/Components/simple_card.dart';
import 'package:good_heart/Components/upper_navigation_bar.dart';
import 'package:good_heart/Components/bottom_navigation_bar.dart';

class DefaultHomePage extends StatefulWidget {
  @override
  _DefaultHomePage createState() => _DefaultHomePage();
}

class _DefaultHomePage extends State<DefaultHomePage> {
  @override
  Widget build(BuildContext context) {

    // FRAME
    var screenSize = MediaQuery.of(context).size;

    final double appBarHeight = screenSize.height * 0.15;

    return Scaffold(
        body: Container(
            width: screenSize.width,
            height: screenSize.height,
            child: Stack(
                 children: <Widget>[
                      ListView(
                          // INSTRUCTIONS
                          children: <Widget> [
                            SimpleCard(
                               top: appBarHeight, 
                               left: 30,
                               right: 30,
                               text: Text("Follow the instructions: \n",
                                  textAlign: TextAlign.center,
                                  style: TextStyle(height: 1.2, fontSize: 20),
                                ),
                            ),
                            SimpleCard(
                               top: 0, 
                               left: 30,
                               right: 30,
                               text: Text("1. Click the “Device” button bellow and then click “Connect to Device”.\n",
                                  textAlign: TextAlign.center,
                                  style: TextStyle(height: 1.2, fontSize: 20),
                                ),
                            ),
                            SimpleCard(
                               top: 0, 
                               left: 30,
                               right: 30,
                               text: Text("If the connection was successful then you can start an evaluation:\n",
                                  textAlign: TextAlign.center,
                                  style: TextStyle(height: 1.2, fontSize: 20),
                                ),
                            ),
                            SimpleCard(
                               top: 0, 
                               left: 30,
                               right: 30,
                               text: Text("2. To start an evaluation of an ECG, click the “Evaluation” button bellow.\n",
                                  textAlign: TextAlign.center,
                                  style: TextStyle(height: 1.2, fontSize: 20),
                                ),
                            ),
                          ],
                    ),

                  // UPPER NAVIGATION BAR
                  UpperAppBar(),

              ]
            )
        ),
        // BOTTOM NAVIGATION BAR
        bottomNavigationBar: BottomNavigationAppBar(),
    );
  }
}

