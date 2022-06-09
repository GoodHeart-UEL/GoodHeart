import socket
import json

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # Cria um socket TCP/IP

server.bind(('localhost',3333)) # Seta o ip e a porta do servidor

server.listen(1) # Configura servidor para escutar 1 cliente

files = [
    { "ECGFileName" : 100 },
    { "ECGFileName" : 101 },
    { "ECGFileName" : 103 },
    { "ECGFileName" : 105 },
    { "ECGFileName" : 106 },
    { "ECGFileName" : 109 },
    { "ECGFileName" : 111 },
    { "ECGFileName" : 118 },
    { "ECGFileName" : 119 },
    { "ECGFileName" : 124 },
    { "ECGFileName" : 214 }
]

while True:
    print('Esperando pela conexao...')
    connection, client_address = server.accept() # Servidor fica esperando para aceitar um cliente

    try:
        print('Cliente conectado: ', client_address)
        while True:
            data = connection.recv(512)
            
            if data:
                message = json.loads(data.decode())
                
                if message['OpCode'] == 500: # Teste de conexão
                    idMsg = -1
                    if "IdMsg" in message:
                        idMsg = message["IdMsg"]
                    idMsg = idMsg + 1
                    response = { "OpCode": 510, "IdMsg": idMsg }
                    connection.sendall(json.dumps(response).encode())
                elif message['OpCode'] == 100: # Avaliacao do ECG
                    idMsg = -1
                    if "IdMsg" in message:
                        idMsg = message["IdMsg"]
                    idMsg = idMsg + 1
                    ecgFile = message['ECGFile']
                    response = { "OpCode": 400, "IdMsg": idMsg, "ECGFile": ecgFile, "GoodComplex": 11, "BadComplex": 2 }
                    connection.sendall(json.dumps(response).encode())
                elif message['OpCode'] == 600: # Listagem dos arquivos
                    connection.sendall(json.dumps(files).encode())
                else:
                    response = { "OpCode": 404 } # Caso o servidor receba uma requisicao não esperada retorna 404
                    connection.sendall(json.dumps(response).encode()
    finally:
        connection.close()


