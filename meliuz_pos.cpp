/*
 * Programa  meliuz_pos.cpp
 *
 * Remaq Comércio e Representações LTDA.
 *
 * Autor: Mario Sergio 
 * <mario@remaqbh.com.br>
 *
 * Junho 2018.
 *
 * Compilação:
 * g++ -DCARREGAMENTO_DINAMICO -Wall -g -fno-strict-aliasing -c "meliuz_pos.cpp" -o "meliuz_pos.o"
 * g++ -o meliuz_pos meliuz_pos.o -lpthread -ldl
 *
 */
#ifdef __unix__                    /* __unix__ is usually defined by compilers targeting Unix systems */

#define OS_Windows 0
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <errno.h>
#include <vector>

#elif defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or   64 bit Windows systems */
#include <stdio_ext.h>
#include <unistd.h> 
#include <getopt.h> 

#endif
#include "getopt.h"

#define MAX_VERSION	1
#define MIN_VERSION	5
#define SIZE 9999

#define ARQUIVO_ESCOLHIDO 1
#define linha 2

//ANSWERS
#define EXIT_SUCESSO 0
#define EXIT_ERRO -1
using namespace std;

//VARIAVEIS GLOBAIS  
ofstream logdir, logrotate, agenda ,ateos, cell;
int i = 0,  cont = 0,  viaPinpad = 0, flag = 0, acao = 1, lixo = 0, idenfifyOnly = 0, CUPOM = 0, status =0, state = 0;
string DATA_ATUAL, trace, command, action, telefone, tecla, cstId ,cellphone ;
char dta[11], dta_log[30], delimit[] = "|", delimit2[] = "/", LOJA[5], PDV[4];
FILE *meliuz_pos, *LISTA, *agendamento, *TRACE_LOG = NULL, *input , *cellAtual;
bool DEBUG = false;

//PROTOTIPOS DE FUNCOES
void criarAgendamento(void);
void pdvLayout(void);
void limpaVariaveis(void);
void inicioVariaveis(void);
void telefoneIdentificado(void);
void IDCliente(void);
void evaluate(void);
void telefoneInvalido(void);
int finalizar(int);
void limpaAteos(void);
void celularCliente(void);
//void geraLog	   		(void);
void vTrace(string, ...);

int finalizar(int tipoEncerramento) {
	if (tipoEncerramento == EXIT_ERRO) {
		vTrace("Encerrado com erro.");
		vTrace("--------------------------------------------------------- Fim.");
		logdir.close();
		cout << "BYE -1" << endl;
		exit(-1);
		return -1;
	}
	else if (tipoEncerramento == EXIT_SUCESSO) {
		vTrace("Encerrado com sucesso.");
		vTrace("--------------------------------------------------------- Fim.");
		logdir.close();
		cout << "BYE 1" << endl;
	}
	else {
		vTrace("Encerrado.");
		vTrace("--------------------------------------------------------- Fim.");
		logdir.close();
		cout << "BYE 1" << endl;
	}
	return 0;
}

void pdvLayout(void){
	switch (status) { 
		case 0:
			cout << "LAYOUT 0" << endl;
		case 1:
			if ( state == 1 ){
				cout << "LAYOUT 1" << endl;
			}
			else if ( state == 2 ){
				cout << "LAYOUT 2" << endl;
			}
			break;
		case 5:	
			cout << "LAYOUT 7" << endl;
			break;
	}

	cout << "COMMAND 111" << endl;
	cin >> lixo;
	setbuf(stdin, NULL);
}

void limpaVariaveis(void){
	logdir << dta_log << " Limpando variaveis. " << endl;
	for (int ct=0;ct <=4;ct++){
		cout << "MENSAGEM" << ct << "= " << endl;
		logdir << dta_log << "MENSAGEM" << ct << "= " << endl;
	}
	setbuf(stdout, 0);
	logdir << dta_log << "------------------------------------------------------------" << endl;
}

void inicioVariaveis(void){
	logdir << dta_log << " - INICIALIZANDO VARIAVEIS MENSAGENS" << endl;
	logdir << dta_log << "   VARIAVEL MENSAGEM0       Meliuz " << endl;
	logdir << dta_log << "   VARIAVEL MENSAGEM1 Dinheiro de volta em todas as compras." << endl;
	logdir << dta_log << "   VARIAVEL MENSAGEM2 " << endl;
	logdir << dta_log << "   VARIAVEL MENSAGEM3 Digite o numero do telefone do cliente" << endl;
	logdir << dta_log << "------------------------------------------------------------" << endl;

	cout << "MENSAGEM0=        Meliuz" << endl;
	cout << "MENSAGEM1= Dinheiro de volta em todas as compras." << endl;
	cout << "MENSAGEM2=  " << endl;
	cout << "MENSAGEM3= Digite o numero do telefone do cliente" << endl;

}

void telefoneIdentificado(void){
	logdir << dta_log << " - TELEFONE IDENTIFICADO" << endl;
	logdir << dta_log << "   VARIAVEL MENSAGEM0       Meliuz " << endl;
	logdir << dta_log << "   VARIAVEL MENSAGEM1 Dinheiro de volta em todas as compras." << endl;
	logdir << dta_log << "   VARIAVEL MENSAGEM2 " << endl;
	logdir << dta_log << "   VARIAVEL MENSAGEM3 Esta venda possui telefone identificado!" << endl;
	logdir << dta_log << "------------------------------------------------------------" << endl;

	cout << "MENSAGEM0=        Meliuz" << endl;
	cout << "MENSAGEM1= Dinheiro de volta em todas as compras." << endl;
	cout << "MENSAGEM2=  " << endl;
	cout << "MENSAGEM3= Esta venda possui telefone identificado!" << endl;
}

void telefoneInvalido(void){
	logdir << dta_log << " - TELEFONE INALIDO" << endl;
	logdir << dta_log << "   VARIAVEL MENSAGEM0       Meliuz " << endl;
	logdir << dta_log << "   VARIAVEL MENSAGEM1 Telefone com formatado invalido." << endl;
	logdir << dta_log << "   VARIAVEL MENSAGEM2 EX: DDD + 9XXXXXXXX" << endl;
	logdir << dta_log << "   VARIAVEL MENSAGEM3 Tentar novamente?" << endl;
	logdir << dta_log << "------------------------------------------------------------" << endl;

	cout << "MENSAGEM0=        Meliuz" << endl;
	cout << "MENSAGEM1= Telefone com formatado invalido." << endl;
	cout << "MENSAGEM2= EX: DDD + 9XXXXXXXX" << endl;
	cout << "MENSAGEM3= Tentar novamente?" << endl;
}

void limpaAteos(void){
	vTrace("Inicio ateos. ");
	remove("/var/venditor/WRK/NEW_AT_EOS.dat");
	FILE *output = fopen("/var/venditor/WRK/NEW_AT_EOS.dat", "w");
	char *token, *retorno, linhaAteos[255];
	int ctrl = 0 , line = 0, ct = 0;
	long t;

	logdir << dta_log << "Limpa ATEOS"  << endl;
	agendamento = fopen("/var/venditor/WRK/AT_EOS.dat", "r");
	if (agendamento){
		while(!feof(agendamento) && i < 255) { 
			retorno = fgets(linhaAteos, 255, agendamento);
			//	printf("Parte : %s\n", retorno);
			if (retorno)
				token = strtok(retorno, delimit2);
			//	printf("token : %s\n", token);
			int j = 0;
			while (token != NULL){
				switch (j) {
					case 1:
						if (strcmp(token, "meliuz_pos") > 0){
							logdir << dta_log << "agendamento encontrado." << " linha " << ct+1 << endl;
							ctrl = 1; 
							line = ct;
						}
						break;
				}
				token = strtok(NULL, delimit2);
				j++;
			}
			ct++;
		}
		fclose(agendamento);
	}
	else{
		logdir << dta_log << "Nao conseguiu abrir ateos."  << endl;
	}
	if ( ctrl == 1 ){
		logdir << dta_log << "ATEOS PARA APAGAR A LINHA [" << line << "]"  << endl;

		agendamento = fopen("/var/venditor/WRK/AT_EOS.dat", "r");
		if (agendamento){
			for (i = 0 ; !feof(agendamento); i++){
				memset(linhaAteos, 0, 255);	
				fgets(linhaAteos, 255, agendamento);
				if ( i == line){
					fputc(' ',output);
					continue;
				}

				fputs(linhaAteos, output);
			}
			fclose(agendamento);
			fclose(output);	
		}

		output = fopen("/var/venditor/WRK/NEW_AT_EOS.dat", "r"); 
		if (output){

			fseek(output, 0, SEEK_END);
			t = ftell(output);
			logdir << dta_log << "Tamanho do Arquivo : " << t << endl;

			rename("/var/venditor/WRK/NEW_AT_EOS.dat", "/var/venditor/WRK/AT_EOS.dat");

			if ( t < 3 ){
				logdir << dta_log << "Apagando ateos . " << endl;
				remove("/var/venditor/WRK/AT_EOS.dat");
			}
			fclose(output);	
		}
	}
}

void celularCliente(void){
	logdir << dta_log << "Funcao celularCleinte."  << endl;
	cellAtual = fopen("/var/venditor/WRK/CELLCLI.dat","r");
	if (cellAtual){
		logdir << dta_log << "Apagando Celular Anterior."  << endl;
		remove("/var/venditor/WRK/CELLCLI.dat");
		fclose(cellAtual);

	}

	cellAtual = fopen("/var/venditor/WRK/CELLCLI.dat","w");
	if (cellAtual){
		logdir << dta_log << "Gravando novo Celular.."  << endl;
		cell.open("/var/venditor/WRK/CELLCLI.dat", ofstream::app);
		if (!cell.is_open()){
			cell.clear();
		}else{
			cell << cstId; 
		}
		fclose(cellAtual);
	}
	else{
		logdir << dta_log << "Nao conseguiu abrir Crtl Celular."  << endl;
	}
}

void criarAgendamento(void) {
	vTrace("Inicio agendar. ");
	char *token2, *retorno, linha2[255];

	agendamento = fopen("/var/venditor/WRK/AT_EOS.dat", "r");
	if (!agendamento) {
		agenda.open("/var/venditor/WRK/AT_EOS.dat", ofstream::app);
		if (!agenda.is_open()) {
			vTrace("Nao foi possivel abrir o arquivo de agendamento.");
			agenda.clear();
		}
		else {
			cout << "AT_EOS SHELL ./meliuz_pos --command=ateos --cell-phone=" << cstId << " --trace=/var/log/meluiz_pos.log\n";
			vTrace("Agendamento criado.");
		}
	}
	else {
		while (!feof(agendamento) && i < 255) { // Loop para ler cada linha do arquivo enquanto houver linhas
			retorno = fgets(linha2, 255, agendamento);
			if (retorno)
				token2 = strtok(retorno, delimit2);
			int j = 0;
			while (token2 != NULL) {
				switch (j) {
					case 1:
						if (strcmp(token2, "meliuz_pos") > 0) {
							vTrace("Ja existe um agendamento criado. ");
						}
						else {
							agenda.open("/var/venditor/WRK/AT_EOS.dat", ofstream::app);
							if (!agenda.is_open()) {
								vTrace("Nao foi possivel abrir o arquivo de agendamento. ");
								agenda.clear();
							}
							cout << "AT_EOS SHELL ./meliuz_pos --command=ateos --cell-phone=" << cstId << " --trace=/var/log/meluiz_pos.log\n";
							vTrace("Agendamento criado. ");
						}
						break;
				}
				//	printf(" %s\n", token2);
				token2 = strtok(NULL, delimit2);
				j++;
			}
		}
	}
}

void evaluate(void){
	cout << "EVALUATE {STATUS}" << endl;
	cin >> status;
	setbuf(stdin, NULL);

	cout << "EVALUATE {STATE}" << endl;
	cin >> state;
	setbuf(stdin, NULL);

	if (action.empty()) {
		vTrace("Carregando action.");
		cout << "EVALUATE {ACTION}" << endl;
		getline(cin, action);
		setbuf(stdin, NULL);
	}
	cout << "EVALUATE {LOJA}" << endl;
	cin >> LOJA;
	setbuf(stdin, NULL);

	cout << "EVALUATE {CAIXA}" << endl;
	cin >> PDV;
	setbuf(stdin, NULL);

	cout << "EVALUATE {CUPOM}" << endl;
	cin >> CUPOM;
	setbuf(stdin, NULL);

	//	cout << "EVALUATE {RESPOSTA[128]}" << endl;
	//	getline(cin, RESP128);
	//	setbuf(stdin, NULL);
}

void IDCliente(void) {
	vTrace("--------------------------------------------------------- Inicio IDCliente. ");

	logdir << dta_log << "**** LOJA: " << LOJA << " - PDV: " << PDV << " - CUPOM: " << CUPOM << " - VERSAO.: " << MAX_VERSION << "." << MIN_VERSION  << " ****"<< endl;

	int resposta = 0;

	if (viaPinpad == 1) {
		while ( flag == 0 ){
			logdir << dta_log << "Acao : [" << acao << "]" << endl;
			switch (acao) {
				case 1:
					inicioVariaveis();	
					cout << "COMMAND 111" << endl;
					cin >> lixo;
					setbuf(stdin, NULL);

					cout << "ACCEPT TITLE Solicite a digitacao do cliente no PinPad. " << endl;
					cout << "ACCEPT PROMPT Informe o Telefene - (PINPAD)" << endl;
					cout << "ACCEPT PIN_MSG DDD + TELEFONE" << endl;
					//cout << "ACCEPT CEL_PINDATA" << endl;
					cout << "ACCEPT PINDATA" << endl;
					getline(cin, telefone);
					setbuf(stdin, NULL);

					logdir << dta_log << "Entrada Pinpad: " << telefone << endl;

					logdir << dta_log << "Tamanho cell informado: " << telefone.size() << endl;

					if (telefone.empty() || telefone.compare("NULL") == 0) {
						vTrace("Telefone nao confirmado. Repetir?");
						acao = 2;	
					}else if ( telefone.size() < 11){ 
						telefoneInvalido();	
						cout << "COMMAND 111" << endl;
						cin >> lixo;
						setbuf(stdin, NULL);
						vTrace("Telefone invalido. Repetir?");
						acao = 3;
					}else if ( !telefone.empty() && telefone.size() > 10 && !telefone.compare("NULL") == 0){
						int cellInvalido = 0;	
						int sizeCell = telefone.size(); 
						
						logdir << dta_log << "Validando Caracteres do Celular... " << endl;
						
						for (int i = 0; i < sizeCell; i++){
						       if(!isdigit(telefone[i])){
							       logdir << dta_log << "------------------------------------------------------------" << endl;
							       logdir << dta_log <<"* Este Digito nao e Decimal: " << telefone[i] << " - ERROR" << endl;
							       cellInvalido = 1;
							       logdir << dta_log << "------------------------------------------------------------" << endl;
						       }else{
							       logdir << dta_log <<"Este Digito e Decimal: " << telefone[i] << " - OK" << endl;

						       }	       
						}
					
			 			cout << "Resultando Cell Valido : " << cellInvalido << endl;

						if ( cellInvalido == 1 ){
							vTrace("Telefone nao confirmado. Repetir?");
							acao = 3;
						}else{
							vTrace("Requisitos OK!");
							cstId = telefone;
							flag = 1;
						}
					}else{
						logdir << dta_log << "Telefone informado: " << telefone << endl;
						cstId = telefone;

						inicioVariaveis();
						pdvLayout();
						cout << "COMMAND 14" << endl;
						cout << "ERROR=Telefone nao informado." << endl;
						finalizar(EXIT_ERRO);	

						flag = 1;
					}
					break;
				case 2:
					inicioVariaveis();
					pdvLayout();
					cout << "COMMAND 14" << endl;
					cout << "ERROR=Telefone nao informado." << endl;
					finalizar(EXIT_ERRO);	

					cout << "ACCEPT YESNO Telefone nao informado. Deseja repetir? " << endl;
					cin >> resposta;
					setbuf(stdin, NULL);
					logdir << dta_log << "Resposta do Telefone nao informado : " << resposta << endl;

					if ( resposta == 1 ){
						acao = 1;
					}else{
						inicioVariaveis();
						pdvLayout();
						cout << "COMMAND 14" << endl;
						cout << "ERROR=Telefone nao informado." << endl;
						finalizar(EXIT_ERRO);	
					}
					break;
				case 3:
					cout << "ACCEPT YESNO Telefone invalido. Deseja repetir? " << endl;
					cin >> resposta;
					setbuf(stdin, NULL);
					logdir << dta_log << "Resposta do invalido : " << resposta << endl;

					if ( resposta == 1 ){
						acao = 1;
					}else{
						inicioVariaveis();
						pdvLayout();
						cout << "COMMAND 14" << endl;
						cout << "ERROR=Telefone invalido." << endl;
						finalizar(EXIT_ERRO);	
					}
					break;
			}
		}	
	}
	else {
		vTrace("Via pinpad [ Nao ]");
		do {
			cout << "ACCEPT TITLE Informe DDD + TELEFONE" << endl;
			cout << "ACCEPT PROMPT Telefone:" << endl;
			cout << "ACCEPT READ" << endl;
			getline(cin, telefone);
			setbuf(stdin, NULL);
			logdir << dta_log << "Entrada via Teclado: " << telefone << endl;
			setbuf(stdin, NULL);
			if (telefone.empty() || telefone.compare("NULL") == 0) {
				vTrace("Telefone nao confirmado. Repetir?");
				cout << "ACCEPT YESNO Telefone nao informado. Deseja repetir? " << endl;
				cin >> resposta;
				setbuf(stdin, NULL);
			}
			else {
				logdir << dta_log << "Telefone informado: " << telefone << endl;
				resposta = 0;
				cstId = telefone;
			}
		} while (resposta != 0);
	}	
}


void vTrace(string texto, ...) {
	//Trabalhando com Datas
	time_t rawtime;
	struct tm *local;
	char dta_log[30];
	time(&rawtime);
	local = localtime(&rawtime);
	strftime(dta_log, 30, "%d-%m-%Y %H:%M:%S - ", local);
	logdir << dta_log << texto << endl;
	//	fprintf(logdir,"%s \n", texto);  
}

void help() {
	printf("Options:\n\n");
	printf("[--version -V]\n");
	printf("[<Output version and exit.>]\n\n");
	printf("[--help -h]\n");
	printf("[<Output help and exit.>]\n\n");
	printf("[--trace -t]\n");
	printf("[<file> Arquivo de log da aplicacao.]\n\n");
	printf("[--cell-phone -P]\n");
	printf("[<Celular Identificado - ateos>]\n\n");
	printf("[--via-pinpad -p]\n");
	printf("[<Entrada de telefone via Pinpad.>]\n\n");
	printf("[identify-only  -i]\n");
	printf("[<Habilita Idenficacao unica de celular.>]\n\n");
	printf("[--command -C]\n");
	printf("[<Comando para funcao de entrada.>]\n\n");
	exit(0);
}

//INICIO --> main()
int main(int argc, char** argv) {
	char optc = 0;
	struct option OpcoesLongas[] = {
		{"trace"  , optional_argument  , NULL, 't'},
		{"command", required_argument  , NULL, 'C'},
		{"via-pinpad", optional_argument  , NULL, 'p'},
		{"cell-phone", optional_argument  , NULL, 'P'},
		{"identify-only", optional_argument , NULL, 'i'},
		{"help"   , no_argument        , NULL, 'h'},
		{"version", no_argument        , NULL, 'V'},
		{        0,                 0,    0,   0}
	};
	if (argc == 1) { // Sem argumentos
		printf("System %s\n", argv[0]);
		printf("Sem arqumentos!\n");
		help();
	}
	while ((optc = getopt_long(argc, argv, "tp:PCi:hV", OpcoesLongas, NULL)) != -1) {
		switch (optc) {
			case 'h':
				printf("System %s\n", argv[0]);
				help();
			case 'V': // Versao
				printf("======================================================\n");
				printf("Desenvolvido por.: Mario Sergio\n");
				printf("Contato..........: mario@remaqbh.com.br\n");
				printf("System %s\n", argv[0]);
				printf("Version %d.%d - Date: [%s %s]\n", MAX_VERSION, MIN_VERSION, __DATE__, __TIME__);
				printf("======================================================\n");
				exit(0);
			case 't': //trace
				trace = optarg;
				break;
			case 'C':
				command = optarg;
				break;
			case 'p': //viaPinpad
				viaPinpad = atoi(optarg);
				break;
			case 'P': //cellphone
				cellphone = optarg;
				break;
			case 'i': //identificacao unica
				idenfifyOnly = atoi(optarg);
				break;
			default: // Qualquer parametro nao tratado
				printf("System %s\n", argv[0]);
				printf("Parametro nao tratado %c \n", optc);
				help();
		}
	}
	meliuz_pos = fopen("/etc/logrotate.d/meliuz_pos", "r");
	if (!meliuz_pos) {
		logrotate.open("/etc/logrotate.d/meliuz_pos");
		logrotate << "/var/log/meliuz_pos*.log {\n";
		logrotate << "    missingok \n";
		logrotate << "    daily \n";
		logrotate << "    rotate 5 \n";
		logrotate << "    compress \n";
		logrotate << " } \n";
		logrotate.close();
	}
	//Trabalhando com Datas
	time_t rawtime;
	struct tm *local;
	time(&rawtime);
	local = localtime(&rawtime);
	//Formatando datas
	strftime(dta, 11, "%Y%m%d", local);
	strftime(dta_log, 30, "%d-%m-%Y %H:%M:%S - ", local);
	DATA_ATUAL = dta;

	if (!trace.empty()) {
		//logdir.open(trace, ofstream::app); nao está funcionando.
		logdir.open("/var/log/meliuz_pos.log", ofstream::app);
		if (!logdir.is_open()) {
			printf(" Nao foi possivel abrir o arquivo de log!\n");
			logdir.clear();
		}
	}
	else {
		logdir.open("/var/log/meliuz_pos.log", ofstream::app);
		if (!logdir.is_open()) {
			printf(" Nao foi possivel abrir o arquivo de log!\n");
			logdir.clear();
		}
	}
	vTrace("--------------------------------------------------------- Inicio.");

	if (command == "identify") {

		int controlCell = 0;
		evaluate();
		logdir << dta_log << " Action..: " << action << endl;
		logdir << dta_log << " Status..: " << status << endl;
		logdir << dta_log << " State...: " << state << endl;

	
		if ( idenfifyOnly == 1 ){
			cellAtual = fopen("/var/venditor/WRK/CELLCLI.dat","r");
			if (cellAtual){
				logdir << dta_log << "Existe arquivo." << endl;
				int i = 0;
				char c ,sizeCell[12];
				string celular;

				while ((c = getc(cellAtual)) != EOF){
					//	 logdir << dta_log << "No while . Letra. " << c << endl;
					sizeCell[i] = c;
					i++;
				}
				sizeCell[i] = '\0';

				celular = sizeCell;
				telefoneIdentificado();

				cout << "COMMAND 111" << endl;
				cin >> lixo;
				setbuf(stdin, NULL);

				cout << "ACCEPT TITLE Telefone [" << celular << "] , ja identificado neste compra." << endl;
				cout << "ACCEPT PROMPT Tecle qualquer tecla para sair." << endl;
				cout << "ACCEPT READ" << endl;
				getline (cin ,tecla);
				setbuf(stdin, NULL);

				controlCell = 1;
			}else{
				controlCell = 0;

			}
		}else{
			controlCell = 0;
		}
		
		if ( controlCell == 0 ){

			acao = 1;
	//		evaluate();
			IDCliente();	
	//		logdir << dta_log << "Action. " << action << endl;
	//		logdir << dta_log << "Status. " << status << endl;
	//		logdir << dta_log << "State. " << state << endl;

			limpaVariaveis();

			if ( cstId.size() > 10 ){

				logdir << dta_log << "------------------------------------------------------------" << endl;
				logdir << dta_log << " - INICIALIZANDO VARIAVEIS MENSAGENS" << endl;
				logdir << dta_log << "   VARIAVEL MENSAGEM0       Meliuz " << endl;
				logdir << dta_log << "   VARIAVEL MENSAGEM1 " << endl;
				logdir << dta_log << "   VARIAVEL MENSAGEM2 Cliente Identificado !!!" << endl;
				logdir << dta_log << "   VARIAVEL MENSAGEM3 " << endl;
				logdir << dta_log << "------------------------------------------------------------" << endl;
				logdir << dta_log << "  CELULAR : " << cstId << endl;
				logdir << dta_log << "------------------------------------------------------------" << endl;

				limpaAteos();
				criarAgendamento();
				celularCliente();

				cout << "MENSAGEM0=        Meliuz" << endl;
				cout << "MENSAGEM1= " << endl;
				cout << "MENSAGEM2=  Cliente Identificado !!!" << endl;
				cout << "MENSAGEM3= " << endl;

				cout << "COMMAND 111" << endl;
				cin >> lixo;
				setbuf(stdin, NULL);

				cout << "ACCEPT TITLE Telefone identificado !!! " << endl;
				cout << "ACCEPT PROMPT Tecle [ENTRA]" << endl;
				cout << "ACCEPT READ" << endl;
				getline (cin ,tecla);
				setbuf(stdin, NULL);
			}
			inicioVariaveis();
		}	
		pdvLayout();	
		cout << "COMMAND 14" << endl;
		finalizar(EXIT_SUCESSO);	
	}
	else if (command == "ateos"){
		evaluate();
		logdir << dta_log << " Action..................:  " << action.c_str() << endl;
	/*	
		if ( action == "OK"){
			logdir << dta_log << "------------------------ AT_EOS ----------------------------" << endl;
			logdir << dta_log << " Celular Identificado :  " << cellphone << endl;
			logdir << dta_log << "------------------------------------------------------------" << endl;
			logdir << dta_log << "INTERNAL ANSWER 128 " << cellphone << endl;
			logdir << dta_log << "STEP ANSWER 128 0 TELEFONE_MELIUZ " << cellphone << endl;
			cout << "INTERNAL ANSWER 128 " << cellphone << endl;
			cout << "STEP ANSWER 128 0 TELEFONE_MELIUZ " << cellphone << endl;
			logdir << dta_log << "------------------------------------------------------------" << endl;
		}
	*/
		cellAtual = fopen("/var/venditor/WRK/CELLCLI.dat","r");
		if (cellAtual){
			logdir << dta_log << "Apagando Celular Anterior."  << endl;
			fclose(cellAtual);
			remove("/var/venditor/WRK/CELLCLI.dat");
		}	
		
		finalizar(EXIT_SUCESSO);
	}
	else if (command == "record"){
		logdir << dta_log << "---------------------- END_SALE ----------------------------" << endl;
		logdir << dta_log << "Verifica Celular" << endl;

		cellAtual = fopen("/var/venditor/WRK/CELLCLI.dat","r");
		if (cellAtual){
			logdir << dta_log << "Existe arquivo." << endl;
			int i = 0;
			char c ,sizeCell[12];
			string celular;

			while ((c = getc(cellAtual)) != EOF){
				//	 logdir << dta_log << "No while . Letra. " << c << endl;
				sizeCell[i] = c;
				i++;
			}
			sizeCell[i] = '\0';

			celular = sizeCell;

			logdir << dta_log << "---------------------- END_SALE ----------------------------" << endl;
			logdir << dta_log << " Celular Identificado :  " << celular << endl;
			logdir << dta_log << "------------------------------------------------------------" << endl;
			logdir << dta_log << "INTERNAL ANSWER 128 " << celular << endl;
			logdir << dta_log << "STEP ANSWER 128 0 TELEFONE_MELIUZ " << celular << endl;
			cout << "INTERNAL ANSWER 128 " << celular << endl;
			cout << "STEP ANSWER 128 0 TELEFONE_MELIUZ " << celular << endl;
			logdir << dta_log << "------------------------------------------------------------" << endl;

			fclose(cellAtual);
			logdir << dta_log << "Apagando Ctrl Celular ."  << endl;
			remove("/var/venditor/WRK/CELLCLI.dat");
		}
		else{
			logdir << dta_log << "Nao existe o arquivo. " << endl;
		}

		finalizar(EXIT_SUCESSO);
	}
	else {
		logdir << dta_log << "Comando invalido. " << command << endl;
	}
	finalizar(EXIT_SUCESSO);
}
