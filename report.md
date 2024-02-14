# Relatório
## Introdução
## Parte 1- Aplicação de download


A Parte 1 consistia no desenvolvimento de uma pequena aplicação que permitisse o download de ficheiros usando o protocolo FTP (como descrito em RFC959), que recebe um argumento que adota a sintaxe URL (como descrito em RCF1738). Este é da forma ftp://[<user>:<password>@]<host>/<url-path>

### Arquitetura da aplicação
A aplicação possui duas funções principais, uma para dar parse ao URL e outra para estabelecer a ligação com o servidor e transferir o ficheiro.

Na função `parse`, processamos o URL passado como argumento através de funções de manipulação de strings (strtok e strrchr), de modo construir uma estrutura de dados com as informações essenciais ao estabelecimento da ligação:
- host: nome do servidor onde será criada a comunicação; 
- path: path até ao ficheiro pretendido; 
- filename: nome e extensão do ficheiro a transferir;
- user e password, para início de sessão no servidor. Como são opcionais, caso estes não sejam especificados ambos ficam com o valor default "anonymous"; 
- ip, obtido através do host com o sistema de tradução DNS.
- ip, obtido pela funnção `getIp`, que traduz o host para um endereço ip através do sistema DNS.

Já a função `download` está encarregue de estabelecer a conexão com o servidor através da criação de um socket com a função `connectSocket`. Caso a conexão tenha sido estabelecida com sucesso, esta função retorna o file descriptor do socket, que usaremos para enviar e receber dados. Inicialmente, é iniciada a sessão com o servidor através do envio do "user" e "password". Entre cada comando enviado lê-se a resposta do servidor com a função `readResponse`. De seguida enviamos o comando "pasv" para o servidor, que nos devolve a porta onde deverá ser estabelecida a conexão para a transferência do ficheiro. Com esta informação, criamos um novo socket, estabelencendo assim outra conexão com o servidor na porta indicada. Por fim, enviamos o comando "retr" para o servidor através do primeiro socket, e é assim iniciada a transferência do ficheiro no outro socket, escrevendo todos os dados recebidos para um ficheiro local. Depois de terminada a transferência, é fechada a conexão com o servidor. 


### Download bem sucedido




<!-- Por exemplo, com
download ftp://ftp.up.pt/pub/filename.txt, o path do URL será ftp://[auth@]domain_and_path, sendo que auth = <username>[:< password>] e domain_and_path = <host>[:<port>]/<path>. -->

### NAT

Por fim, o Tux53 fez  ping ao router do laboratório, onde apenas obteve resposta quando a NAT estava ativa. Isto deve-se ao facto de, sem a NAT, o router não saber a que máquina deve reencaminhar os pacotes, visto que o endereço IP do Tux53 é privado e não é visível fora da rede local.



## Experiência 6 - Aplicação FTP

Através da análise dos logs desta experiência, tira-se que a aplicação abre duas conexões TCP. 
Uma conexão TCP é composta por 3 fases: estabelecimento, transferência de dados e término.
O estabelecimento dessas conexões é feito através de um 3-way handshake, que consiste na troca de 3 pacotes entre o cliente e o servidor. O primeiro pacote é enviado pelo cliente, e contém o flag SYN. O servidor responde com um pacote SYN/ACK, e o cliente responde com um pacote ACK. A conexão é estabelecida quando o servidor recebe o pacote ACK. [Link]
O término da conexão é feito através dos pacotes com as flags FIN e ACK. [Link]
A primeira conexão, a de controlo, é estabelecida na porta 21 (default do protocolo FTP), e é usada para enviar comandos e receber respostas. A segunda conexão, na porta 44044, é usada para a transferência do ficheiro. Ambas são para o mesmo endereço IP, obtido através do sistema DNS.
As conexões TCP utilizam ARQ(Automatic Repeat reQuest). Este mecanismo envolve os campos ACK e SEQ dos pacotes, de modo a garantir uam entrega ordenada e sem erros. O campo ACK é usado para confirmar a receção de um pacote. Caso o pacote não seja recebido, o emissor reenvia-o. Já o campo SEQ é usado para garantir a ordem de entrega dos pacotes. Podemos ver o funcionamento deste mecanismo no wireshark, olhando para os valores dos campos ACK e SEQ dos pacotes.
<!-- Is the throughput of a TCP data connections disturbed by the appearance of a 
second TCP connection? How?
 -->
Era de esperar que, após o início da transferência do ficheiro no segundo computador (Tux53) a taxa de pacotes/seg do Tux52 sofreria uma redução para cerca de metade, estabilizando à volta desse valor.  Contudo, como é possível analisar pelo gráfico da transferência em simultâneo, apesar de haver a redução para valores próximos do esperado, não existe a estabilização. Dito isto, é na mesma possível concluir que a taxa de transferência de uma conexão TCP é afetada com a aparecimento de outra em simultâneo. 




