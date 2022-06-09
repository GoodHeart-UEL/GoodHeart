import 'package:flutter/material.dart';
 
class SimpleCard extends StatelessWidget {
  final double top;
  final double left;
  final double right;
  final Text text;

  const SimpleCard({ Key? key, required this.top, required this.left, required this.right, required this.text, }) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Card(
    child: Padding(
      padding: EdgeInsets.only(top: top, left: left, right: right),
      child: text,
    ),
    shadowColor: const Color.fromRGBO(255, 255, 255, 0),
    color: Colors.grey[50],
  );
  }
}
 
 