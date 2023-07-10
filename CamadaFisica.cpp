#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "CamadaFisica.h"

using namespace std;

int tipoDeCodificacao = 0;

vector<int> ASCIIparaBits(const string& mensagem) {
    vector<int> quadro;

    for (char c : mensagem) {
        for (int i = 7; i >= 0; i--) {
            int bit = (c >> i) & 1;
            quadro.push_back(bit);
        }
    }

    return quadro;
}

string BitsparaASCII(const vector<int>& quadro) {
    string mensagem;
    int asciiChar = 0;
    int bitIndex = 7;

    for (int bit : quadro) {
        asciiChar |= bit << bitIndex;
        bitIndex--;

        if (bitIndex < 0) {
            mensagem.push_back(static_cast<char>(asciiChar));
            asciiChar = 0;
            bitIndex = 7;
        }
    }

    return mensagem;
}

void AplicacaoTransmissora () {
	string mensagem;
	cout << "Digite uma mensagem:" << endl;
    getline(cin, mensagem);
    cout << endl
         << "Qual codificação será utilizada no enlace físico: (default binária)" << endl
         << "0) Codificação Binária" << endl
         << "1) Codificação Manchester" << endl
         << "2) Codificação Bipolar" << endl;
    cin >> tipoDeCodificacao;

	CamadaDeAplicacaoTransmissora(mensagem);
}

void CamadaDeAplicacaoTransmissora (const string& mensagem) {
    vector<int> quadro = ASCIIparaBits(mensagem);

	CamadaFisicaTransmissora(quadro);
}

void CamadaFisicaTransmissora(const vector<int>& quadro){
    vector<int> fluxoBrutoDeBits;

	switch (tipoDeCodificacao) {
		case 0:
			fluxoBrutoDeBits = CamadaFisicaTransmissoraCodificacaoBinaria(quadro);
			break;
		case 1:
			fluxoBrutoDeBits = CamadaFisicaTransmissoraCodificacaoManchester(quadro);
			break;
		case 2:
			fluxoBrutoDeBits = CamadaFisicaTransmissoraCodificacaoBipolar(quadro);
			break;
        default:
            fluxoBrutoDeBits = CamadaFisicaTransmissoraCodificacaoBinaria(quadro);
            break;
	}

    MeioDeComunicacao(fluxoBrutoDeBits);
}

vector<int> CamadaFisicaTransmissoraCodificacaoBinaria(const vector<int>& quadro){
    vector<int> fluxoBrutoDeBits;
    cout << "Codificacao Binária" << endl;
    for (int bit : quadro) {
        cout << bit ;
        fluxoBrutoDeBits.push_back(bit);
    }
    cout << endl;

    return fluxoBrutoDeBits;
}

vector<int> CamadaFisicaTransmissoraCodificacaoManchester(const vector<int>& quadro){
    vector<int> fluxoBrutoDeBits;

    cout << "Codificacao Manchester" << endl;

    int clock = 0; // Valor inicial do clock

    for (int bit : quadro) {
        int encodedBit1 = (bit ^ clock);  // XOR entre o bit e o clock
        clock = (clock ^ 1);
        int encodedBit2 = (bit ^ clock);    // XOR entre o clock e o valor complementar (1)
        clock = (clock ^ 1);
        cout << encodedBit1 << encodedBit2;

        fluxoBrutoDeBits.push_back(encodedBit1);
        fluxoBrutoDeBits.push_back(encodedBit2);
    }
    cout << endl;

    return fluxoBrutoDeBits;
}

vector<int> CamadaFisicaTransmissoraCodificacaoBipolar(const vector<int>& quadro){
    vector<int> fluxoBrutoDeBits;
    cout << "Codificacao Bipolar" << endl;

    int Polaridade = 1; // Polaridade inicial

    for (int bit : quadro) {
        if (bit == 0) {
            fluxoBrutoDeBits.push_back(0);
            cout << 0;
        } else {
            fluxoBrutoDeBits.push_back(Polaridade);
            cout << Polaridade;
            Polaridade = -Polaridade; // Alterna a polaridade
        }
    }
    cout << endl;

    return fluxoBrutoDeBits;
}

void MeioDeComunicacao(vector<int> fluxoBrutoDeBits){
    vector<int> fluxoBrutoDeBitsPontoA = std::move(fluxoBrutoDeBits);
    vector<int> fluxoBrutoDeBitsPontoB;
    int erro,porcentagemDeErros;
    porcentagemDeErros = 0;


    while (fluxoBrutoDeBitsPontoB.size() != fluxoBrutoDeBitsPontoA.size()) {
        for (int bit : fluxoBrutoDeBitsPontoA) {

            fluxoBrutoDeBitsPontoB.push_back(bit);
        }
    }

	CamadaFisicaReceptora(fluxoBrutoDeBitsPontoB);
}

void CamadaFisicaReceptora(const vector<int>& quadro){
	vector<int> fluxoBrutoDeBits;

	switch(tipoDeCodificacao){
		case 0:
			fluxoBrutoDeBits = CamadaFisicaReceptoraDecodificacaoBinaria(quadro);
			break;
		case 1:
			fluxoBrutoDeBits = CamadaFisicaReceptoraDecodificacaoManchester(quadro);
			break;
		case 2:
			fluxoBrutoDeBits = CamadaFisicaReceptoraDecodificacaoBipolar(quadro);
			break;
        default:
            fluxoBrutoDeBits = CamadaFisicaReceptoraDecodificacaoBinaria(quadro);
            break;
	}

	CamadaDeAplicacaoReceptora(fluxoBrutoDeBits);
}

vector<int> CamadaFisicaReceptoraDecodificacaoBinaria(const vector<int>& quadro){
    vector<int> fluxoBrutoDeBits;
    cout << "Decodificacao Binária" << endl;
    for (int bit : quadro) {
        cout << bit;
        fluxoBrutoDeBits.push_back(bit);
    }
    cout << endl;

    return fluxoBrutoDeBits;
}

vector<int> CamadaFisicaReceptoraDecodificacaoManchester(vector<int> quadro){
    vector<int> fluxoBrutoDeBits;

    cout << "Decodificacao Manchester" << endl;

    int clock = 0;

    for (int i = 0; i < quadro.size(); i += 2) {
        int bit1 = quadro[i];
        int bit2 = quadro[i + 1];

        int decodedBit = bit1 ^ clock;

        clock = (clock ^ 1);

        if (decodedBit == (bit2 ^ clock)) {
            cout << decodedBit;
            fluxoBrutoDeBits.push_back(decodedBit);
        } else {
            cout << "ERRO";//erro
            // Tratamento de erro - inserir algum valor indicando a ocorrência de erro
            fluxoBrutoDeBits.push_back(-1); // Por exemplo, inserindo -1 para indicar erro
        }

        clock = (clock ^ 1);
    }
    cout << endl;

    return fluxoBrutoDeBits;
}

vector<int> CamadaFisicaReceptoraDecodificacaoBipolar(const vector<int>& quadro){
    vector<int> fluxoBrutoDeBits;
    cout << "Decodificacao Bipolar" << endl;
    int Polaridade = 1; // Polaridade esperada
    for (int bit : quadro) {
        if (bit != 0 && bit != Polaridade) {
            cout << "Erro!" << endl;
            break;
        }
        if (bit == 0) {
            cout << 0;
            fluxoBrutoDeBits.push_back(0);
        } else {
            cout << 1;
            fluxoBrutoDeBits.push_back(1);
            Polaridade = -Polaridade;
        }
    }
    cout << endl;

    return fluxoBrutoDeBits;
}

void CamadaDeAplicacaoReceptora(const vector<int>& quadro){
    string mensagem = BitsparaASCII(quadro);

	AplicacaoReceptora(mensagem);
}

void AplicacaoReceptora(const string& mensagem){
	cout << "A mensagem recebida foi:" << mensagem << endl;
}