import 'package:flutter/material.dart';

Future<void> infoDialog(BuildContext context, Text text, Text buttonText, Icon icon) async {
  return await showDialog(context: context,
      builder: (context){
        return AlertDialog(
          title: icon,
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