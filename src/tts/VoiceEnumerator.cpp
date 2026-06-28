#include "pch.h"
#include "VoiceEnumerator.h"

HRESULT CVoiceEnumerator::EnumerateInstalledVoices(std::vector<std::pair<std::wstring, std::wstring>>& outVoices) {
    // outVoices: vector de pares <TokenID, DisplayName>
    CComPtr<IEnumSpObjectTokens> spEnum;

    // Enumerar todas las voces en la categoria SPCAT_VOICES
    HRESULT hr = SpEnumTokens(
        SPCAT_VOICES, 
        NULL, // Sin atributos requeridos
        NULL, // Sin atributos opcionales
        &spEnum
    );
    
    if (FAILED(hr)) return hr;

    // Iterar sobre los tokens encontrados
    while (hr == S_OK) {
        CComPtr<ISpObjectToken> spToken;
        ULONG celtFetched = 0;
        hr = spEnum->Next(1, &spToken, &celtFetched);
        if (FAILED(hr) || celtFetched == 0) break;

        // Obtener el Token ID completo (para SetVoice y persistencia)
        LPWSTR pszTokenId = NULL;
        if (SUCCEEDED(spToken->GetId(&pszTokenId))) {
            std::wstring tokenId(pszTokenId);
            CoTaskMemFree(pszTokenId);

            // Obtener el nombre descriptivo de la voz usando SpGetDescription
            LPWSTR pszName = NULL;
            if (SUCCEEDED(SpGetDescription(spToken, &pszName))) {
                std::wstring name(pszName);
                ::CoTaskMemFree(pszName);

                // Opcional: obtener idioma y genero
                LPWSTR pszLang = NULL;
                if (SUCCEEDED(spToken->GetStringValue(L"Language", &pszLang))) {
                    // pszLang contiene el LANGID (ej. "409" para en-US)
                    CoTaskMemFree(pszLang);
                }

                outVoices.emplace_back(tokenId, name);
            }
        }
    }
    
    return S_OK;
}
