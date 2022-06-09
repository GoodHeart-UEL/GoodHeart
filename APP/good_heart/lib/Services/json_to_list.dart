
import 'dart:convert';
import 'package:logger/logger.dart';
import 'package:good_heart/Services/communication_with_server.dart';

var logger = Logger(
  filter: null,
  printer: PrettyPrinter(),
  output: null,
);

var LoggerFunction = "jsonToList";

jsonToList(String response) {    
  var responseDecoded = CommunicationWithServer.fromJson(jsonDecode(response));
  var files = responseDecoded.Files as List;
  logger.d("[$LoggerFunction] Response from server files: $files");

  return files;
}