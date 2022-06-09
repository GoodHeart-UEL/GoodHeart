import 'package:flutter/material.dart';
import 'package:flutter/widgets.dart';

class MyColors {
    static const MaterialColor green = MaterialColor(
    0xFFa4c47c, // AQUI meio aleatório 
    <int, Color>{
      // Green: Color.fromRGBO(164, 196, 124, 1)
      50:Color.fromRGBO(164,196,124, .1),
      100:Color.fromRGBO(164,196,124, .2),
      200:Color.fromRGBO(164,196,124, .3),
      300:Color.fromRGBO(164,196,124, .4),
      400:Color.fromRGBO(164,196,124, .5),
      500:Color.fromRGBO(164,196,124, .6),
      600:Color.fromRGBO(164,196,124, .7),
      700:Color.fromRGBO(164,196,124, .8),
      800:Color.fromRGBO(164,196,124, .9),
      900:Color.fromRGBO(164,196,124, 1),
    },
  );

    // Red: Color.fromRGBO(207, 62, 85, 1)
    static const MaterialColor red = MaterialColor(
    0xFFc53a51,
    <int, Color>{
      50:Color.fromRGBO(207,62,85, .1),
      100:Color.fromRGBO(207,62,85, .2),
      200:Color.fromRGBO(207,62,85, .3),
      300:Color.fromRGBO(207,62,85, .4),
      400:Color.fromRGBO(207,62,85, .5),
      500:Color.fromRGBO(207,62,85, .6),
      600:Color.fromRGBO(207,62,85, .7),
      700:Color.fromRGBO(207,62,85, .8),
      800:Color.fromRGBO(207,62,85, .9),
      900:Color.fromRGBO(207,62,85, 1),
    },
  );

}