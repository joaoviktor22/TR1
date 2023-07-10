#include "CamadaFisica.h"
#include "CamadaEnlace.h"

using namespace std;

void CamadaEnlaceDadosTransmissora(const vector<int>& quadro){
    CamadaEnlaceDadosTransmissoraEnquadramento(quadro);
    CamadaEnlaceDadosTransmissoraControleDeErro(quadro);
}

void CamadaEnlaceDadosTransmissoraEnquadramento(const vector<int>& quadro){
    int tipoDeEnquadramento = 0;
    vector<int> quadroEnquadrado;
    switch (tipoDeEnquadramento) {
        case 0: //bit de paridade par
            quadroEnquadrado = CamadaEnlaceDadosTransmissoraEnquadramentoContagemDeCaracteres(quadro);
            break;
        case 1 : //CRC
            quadroEnquadrado = CamadaEnlaceDadosTransmissoraEnquadramentoInsercaoDeBytes(quadro);
            break;
        default :
            quadroEnquadrado = CamadaEnlaceDadosTransmissoraEnquadramentoContagemDeCaracteres(quadro);
            break;
    }
}

void CamadaEnlaceDadosTransmissoraControleDeErro(const vector<int>& quadro){
    int tipoDeControleDeErro = 0;
    switch (tipoDeControleDeErro) {
        case 0: //bit de paridade par
            break;
        case 1 : //CRC
            //codigo
            break;
        case 2: //codigo de Hamming
            break;
        default:
            //bit de paridade par
            break;
    }
}

vector<int> CamadaEnlaceDadosTransmissoraEnquadramentoContagemDeCaracteres(const vector<int>& quadro){
    vector<int> quadroEnquadrado;

    int tamanhoQuadroBytes = quadro.size() / 8; // Tamanho do quadro em bytes

    // Converter o tamanho do quadro em bits para um vetor de bits
    vector<int> tamanhoQuadroBits;
    for (int i = 31; i >= 0; i--) {
        int bit = (tamanhoQuadroBytes >> i) & 1;
        tamanhoQuadroBits.push_back(bit);
    }

    // Adicionar o tamanho do quadro no cabeçalho
    for (int bit : tamanhoQuadroBits) {
        quadroEnquadrado.push_back(bit);
    }

    // Adicionar os bits do quadro original
    for (int bit : quadro) {
        quadroEnquadrado.push_back(bit);
    }

    return quadroEnquadrado;
}

vector<int> CamadaEnlaceDadosTransmissoraEnquadramentoInsercaoDeBytes(const vector<int>& quadro){
    vector<int> quadroEnquadrado;

    // Byte de flag em bits (exemplo: 01111110)
    vector<int> flagBits = {0, 1, 1, 1, 1, 1, 1, 0};

    // Adicionar o byte de flag no início do quadro
    quadroEnquadrado.insert(quadroEnquadrado.end(), flagBits.begin(), flagBits.end());

    // Adicionar o quadro original
    quadroEnquadrado.insert(quadroEnquadrado.end(), quadro.begin(), quadro.end());

    // Adicionar o byte de flag no final do quadro
    quadroEnquadrado.insert(quadroEnquadrado.end(), flagBits.begin(), flagBits.end());

    return quadroEnquadrado;
}

vector<int> CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(const vector<int>& quadro){
    vector<int> quadroComParidade = quadro;

    // Contar o número de bits 1 no quadro
    int numBits1 = count(quadro.begin(), quadro.end(), 1);

    // Calcular o bit de paridade (0 se o número de bits 1 for par, 1 se for ímpar)
    int bitParidade = (numBits1 % 2 == 0) ? 0 : 1;

    // Adicionar o bit de paridade ao quadro
    quadroComParidade.push_back(bitParidade);

    return quadroComParidade;
}

vector<int> CamadaEnlaceDadosTransmissoraControleDeErroCRC(const vector<int>& quadro){
    // Polinômio gerador
    vector<int> gerador = {1, 0, 1, 1};  // Exemplo de polinômio gerador: x^3 + 1 (representado como 1011)

    // Adicionar zeros ao final do quadro para acomodar o CRC
    vector<int> quadroComZeros = quadro;
    quadroComZeros.insert(quadroComZeros.end(), gerador.size() - 1, 0);

    // Realizar a operação de divisão (XOR) entre o quadro e o polinômio gerador
    for (int i = 0; i <= quadroComZeros.size() - gerador.size(); i++) {
        if (quadroComZeros[i] == 1) {
            for (int j = 0; j < gerador.size(); j++) {
                quadroComZeros[i + j] ^= gerador[j];
            }
        }
    }

    // O resto da divisão (resultado final) é o CRC
    vector<int> crc(gerador.size() - 1);
    copy(quadroComZeros.end() - crc.size(), quadroComZeros.end(), crc.begin());

    // Adicionar o CRC ao final do quadro
    vector<int> quadroComCRC = quadro;
    quadroComCRC.insert(quadroComCRC.end(), crc.begin(), crc.end());

    return quadroComCRC;
}

vector<int> CamadaEnlaceDadosTransmissoraControleDeErroCodigoDeHamming(const vector<int>& quadro){
    int m = quadro.size();  // Número de bits de dados
    int r = 0;              // Número de bits de redundância

    // Determinar o número de bits de redundância necessários
    while ((1 << r) < m + r + 1) {
        r++;
    }

    // Calcular a posição dos bits de redundância
    vector<int> posicoesBitsRedundancia;
    for (int i = 0; i < r; i++) {
        posicoesBitsRedundancia.push_back((1 << i) - 1);
    }

    // Criar o quadro com os bits de dados e os bits de redundância
    vector<int> quadroComRedundancia;
    int posQuadro = 0;
    int posRedundancia = 0;
    for (int i = 0; i < m + r; i++) {
        if (i == posicoesBitsRedundancia[posRedundancia]) {
            quadroComRedundancia.push_back(0);  // Colocar um bit de redundância 0 temporariamente
            posRedundancia++;
        } else {
            quadroComRedundancia.push_back(quadro[posQuadro]);
            posQuadro++;
        }
    }

    // Calcular os bits de paridade
    for (int i = 0; i < r; i++) {
        int posicaoBitRedundancia = (1 << i) - 1;
        int paridade = 0;
        for (int j = 0; j < m + r; j++) {
            if (((j + 1) & (posicaoBitRedundancia + 1)) != 0) {
                paridade ^= quadroComRedundancia[j];
            }
        }
        quadroComRedundancia[posicaoBitRedundancia] = paridade;
    }

    return quadroComRedundancia;
}

void CamadaEnlaceDadosReceptora(const vector<int>& quadro){
    CamadaEnlaceDadosReceptoraEnquadramento(quadro);
    CamadaEnlaceDadosReceptoraControleDeErro(quadro);

    CamadaDeAplicacaoReceptora(quadro);
}

void CamadaEnlaceDadosReceptoraEnquadramento(const vector<int>& quadro){
    int tipoDeEnquadramento = 0;
    vector<int> quadroEnquadrado;
    switch (tipoDeEnquadramento) {
        case 0: //bit de paridade par
            quadroEnquadrado = CamadaEnlaceDadosReceptoraEnquadramentoContagemDeCaracteres(quadro);
            break;
        case 1 : //CRC
            quadroEnquadrado = CamadaEnlaceDadosReceptoraEnquadramentoInsercaoDeBytes(quadro);
            break;
        default:
            quadroEnquadrado = CamadaEnlaceDadosReceptoraEnquadramentoContagemDeCaracteres(quadro);
            break;
    }
}

void CamadaEnlaceDadosReceptoraControleDeErro(const vector<int>& quadro){
    int tipoDeControleDeErro = 0;
    switch (tipoDeControleDeErro) {
        case 0: //bit de paridade par
            break;
        case 1 : //CRC
            //codigo
            break;
        case 2: //codigo de Hamming
            break;
        default:
            break;
    }
}

vector<int> CamadaEnlaceDadosReceptoraControleDeErroBitParidadePar(const vector<int>& quadro){

}

vector<int> CamadaEnlaceDadosReceptoraControleDeErroCRC(const vector<int>& quadro){

}

vector<int> CamadaEnlaceDadosReceptoraControleDeErroCodigoDeHamming(const vector<int>& quadro){

}

vector<int> CamadaEnlaceDadosReceptoraEnquadramentoContagemDeCaracteres(const vector<int>& quadro){
    vector<int> quadroOriginal;

    // Verificar se há bits suficientes para o tamanho do quadro no cabeçalho
    if (quadro.size() >= 32) {
        // Extrair o tamanho do quadro em bytes do cabeçalho
        int tamanhoQuadroBytes = 0;
        for (int i = 0; i < 32; i++) {
            tamanhoQuadroBytes = (tamanhoQuadroBytes << 1) | quadro[i];
        }

        // Verificar se há bits suficientes para o quadro completo (incluindo o cabeçalho)
        if (quadro.size() >= 32 + tamanhoQuadroBytes * 8) {
            // Extrair os bits do quadro original
            for (int i = 32; i < 32 + tamanhoQuadroBytes * 8; i++) {
                quadroOriginal.push_back(quadro[i]);
            }
        }
    }

    return quadroOriginal;
}

vector<int> CamadaEnlaceDadosReceptoraEnquadramentoInsercaoDeBytes(const vector<int>& quadro){
    vector<int> quadroOriginal;

    // Byte de flag em bits (01111110)
    vector<int> flagBits = {0, 1, 1, 1, 1, 1, 1, 0};

    // Verificar se o quadro começa e termina com o byte de flag
    if (quadro.size() >= 2 * flagBits.size() && equal(quadro.begin(), quadro.begin() + flagBits.size(), flagBits.begin()) && equal(quadro.rbegin(), quadro.rbegin() + flagBits.size(), flagBits.rbegin())) {
        // Remover o byte de flag do início e fim do quadro
        auto quadroSemFlag = vector<int>(quadro.begin() + flagBits.size(), quadro.end() - flagBits.size());

        // Adicionar o quadro sem flags ao quadro original
        quadroOriginal.insert(quadroOriginal.end(), quadroSemFlag.begin(), quadroSemFlag.end());
    }

    return quadroOriginal;
}