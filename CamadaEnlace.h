//
// Created by DELL on 06/07/2023.
//

#ifndef P2_CAMADAENLACE_H
#define P2_CAMADAENLACE_H
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

void CamadaEnlaceDadosTransmissora(const vector<int>& quadro);
void CamadaEnlaceDadosTransmissoraEnquadramento(const vector<int>& quadro);
void CamadaEnlaceDadosTransmissoraControleDeErro(const vector<int>& quadro);
vector<int> CamadaEnlaceDadosTransmissoraEnquadramentoContagemDeCaracteres(const vector<int>& quadro);
vector<int> CamadaEnlaceDadosTransmissoraEnquadramentoInsercaoDeBytes(const vector<int>& quadro);
vector<int> CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(const vector<int>& quadro);
vector<int> CamadaEnlaceDadosTransmissoraControleDeErroCRC(const vector<int>& quadro);
vector<int> CamadaEnlaceDadosTransmissoraControleDeErroCodigoDeHamming(const vector<int>& quadro);
void CamadaEnlaceDadosReceptora(const vector<int>& quadro);
void CamadaEnlaceDadosReceptoraEnquadramento(const vector<int>& quadro);
void CamadaEnlaceDadosReceptoraControleDeErro(const vector<int>& quadro);
vector<int> CamadaEnlaceDadosReceptoraControleDeErroBitParidadePar(const vector<int>& quadro);
vector<int> CamadaEnlaceDadosReceptoraControleDeErroCRC(const vector<int>& quadro);
vector<int> CamadaEnlaceDadosReceptoraControleDeErroCodigoDeHamming(const vector<int>& quadro);
vector<int> CamadaEnlaceDadosReceptoraEnquadramentoContagemDeCaracteres(const vector<int>& quadro);
vector<int> CamadaEnlaceDadosReceptoraEnquadramentoInsercaoDeBytes(const vector<int>& quadro);


#endif //P2_CAMADAENLACE_H
