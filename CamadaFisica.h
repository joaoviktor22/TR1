//
// Created by DELL on 21/06/2023.
//

#ifndef P2_CAMADAFISICA_H
#define P2_CAMADAFISICA_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

void AplicacaoTransmissora();
void CamadaDeAplicacaoTransmissora(const string& mensagem);
void CamadaFisicaTransmissora(const vector<int>& quadro);
vector<int> CamadaFisicaTransmissoraCodificacaoBinaria(const vector<int>& quadro);
vector<int> CamadaFisicaTransmissoraCodificacaoManchester(const vector<int>& quadro);
vector<int> CamadaFisicaTransmissoraCodificacaoBipolar(const vector<int>& quadro);
void MeioDeComunicacao(vector<int> fluxoBrutoDeBits);
void CamadaFisicaReceptora(const vector<int>& quadro);
vector<int> CamadaFisicaReceptoraDecodificacaoBinaria(const vector<int>& quadro);
vector<int> CamadaFisicaReceptoraDecodificacaoManchester(vector<int> quadro);
vector<int> CamadaFisicaReceptoraDecodificacaoBipolar(const vector<int>& quadro);
void CamadaDeAplicacaoReceptora(const vector<int>& quadro);
void AplicacaoReceptora(const string& mensagem);

#endif //P2_CAMADAFISICA_H
