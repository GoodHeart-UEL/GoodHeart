import 'package:flutter/material.dart';

Future<void> alertDialog(BuildContext context, Text text, Text buttonText) async {
    return await showDialog(context: context,
        builder: (context){
          return AlertDialog(
            content: text,
            actions: <Widget> [
              TextButton(
                  onPressed: (){
                    Navigator.of(context).pop();
                  },
                  child: buttonText,
              )


            ]
          );
    });
  }