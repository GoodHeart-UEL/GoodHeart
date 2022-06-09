import 'package:flutter/material.dart';
import 'package:good_heart/Theme/colors.dart';

class UpperAppBar extends StatelessWidget {
  const UpperAppBar({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    Size screenSize = MediaQuery.of(context).size;
    final double appBarHeight = screenSize.height * 0.15;
    return Padding(
      padding: EdgeInsets.only(
        top: 0,
      ),
      child: Container(
            width: screenSize.width,
            height: appBarHeight, // AQUI
            decoration: BoxDecoration(
              color: MyColors.green,
            ),
              child: Padding(
                        padding: EdgeInsets.only(
                          left: screenSize.width * 0.05,
                          top: screenSize.height * 0.05,
                        ),
                        child: Row(
                          children: <Widget>[
                            Container(
                              width: screenSize.width * 0.2,
                              height: screenSize.height * 0.2, // AQUI
                              decoration: BoxDecoration(
                                  image: const DecorationImage(
                                // AQUI pesquisar como fazer com path relativo
                                image: AssetImage(
                                    "assets/images/AppIcon/GreenIconGoodHeartNoBackground.png"),
                              )),
                            ),
                            Container(
                                padding: EdgeInsets.only(
                                  left: 5,
                                ),
                                child: Center(
                                    child: Text(
                                  'Good Heart',
                                  style: TextStyle(
                                      //fontFamily: 'ComicSans', //AQUI ver como trocar fonte
                                      color: Colors.black,
                                      fontWeight: FontWeight.bold,
                                      fontSize: 24),
                                )))
                          ],
                        )
                    )
          )
      );
  }
}



