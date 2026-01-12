import 'package:flutter/material.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      title: 'Flutter Web Test',
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Mon App Flutter Web 🌐'),
          backgroundColor: Colors.indigo,
        ),
        body: const Center(
          child: Text(
            'Hello Flutter Web 👋',
            style: TextStyle(fontSize: 26),
          ),
        ),
      ),
    );
  }
}
