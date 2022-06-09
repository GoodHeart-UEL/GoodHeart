import 'package:flutter/material.dart';
import 'package:good_heart/Theme/colors.dart';

class BottomNavigationAppBar extends StatelessWidget {
  const BottomNavigationAppBar({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return BottomNavigationBar(
          onTap: (value) {
            if (value == 1) Navigator.pushNamed(context, '/connection_page');
            if (value == 2) Navigator.pushNamed(context, '/evaluation_page');
            if (value == 3) Navigator.pushNamed(context, '/settings_page');
          },
          items: const <BottomNavigationBarItem>[
            BottomNavigationBarItem(
              icon: Icon(Icons.home),
              label: 'Home',
            ),
            BottomNavigationBarItem(
              icon: Icon(Icons.router_rounded),
              label: 'Device',
            ),
            BottomNavigationBarItem(
              icon: Icon(Icons.folder),
              label: 'Evaluation',
            ),
            BottomNavigationBarItem(
            icon: Icon(Icons.settings),
            label: 'Settings',
            ),
          ],
          showUnselectedLabels: true,
          unselectedItemColor: MyColors.green,
          selectedItemColor: MyColors.red,
    );
  }
}



