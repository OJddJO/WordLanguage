import * as vscode from 'vscode';

const tokenTypes = new Map<string, number>();
const tokenModifiers = new Map<string, number>();

const legend = (function() {
	const tokenTypesLegend = [
		'comment', 'string', 'keyword', 'number', 'regexp', 'operator', 'namespace',
		'type', 'struct', 'class', 'interface', 'enum', 'typeParameter', 'function',
		'method', 'decorator', 'macro', 'variable', 'parameter', 'property', 'label'
	];
	tokenTypesLegend.forEach((tokenType, index) => tokenTypes.set(tokenType, index));

	const tokenModifiersLegend = [
		'declaration', 'documentation', 'readonly', 'static', 'abstract', 'deprecated',
		'modification', 'async'
	];
	tokenModifiersLegend.forEach((tokenModifier, index) => tokenModifiers.set(tokenModifier, index));

	return new vscode.SemanticTokensLegend(tokenTypesLegend, tokenModifiersLegend);
})();

const provider: vscode.DocumentSemanticTokensProvider = {
	provideDocumentSemanticTokens(
		document: vscode.TextDocument
	): vscode.ProviderResult<vscode.SemanticTokens> {
		// analyze the document and return semantic tokens

		const tokensBuilder = new vscode.SemanticTokensBuilder(legend);
		const text = document.getText();
		const lines = text.split("\n");
		let vars: string[] = [];
		let functions: string[] = [];
		let funcArgs: string[] = [];

		for (let lineIndex = 0; lineIndex < lines.length; lineIndex++) {
			const line: string = lines[lineIndex];
			const words: string[] = line.split(/\s+/); // splits by any whitespace

			const wordIndexes: number[] = [];
			let currentIndex = 0;
			for (const word of words) { // get indexes of each words
				const wordIndex = line.indexOf(word, currentIndex);
				wordIndexes.push(wordIndex);
				currentIndex = wordIndex + word.length;
			}

			let isString = false; // if evaluating string

			for (let wordIndex = 0; wordIndex < words.length; wordIndex++) {
				if (!isString && words[wordIndex].startsWith('#')) { //comment
					break;
				}
				if (words[wordIndex].startsWith('"')) { //if the word starts with " then it's a string
					tokensBuilder.push(lineIndex, wordIndexes[wordIndex], words[wordIndex].length, getTokenType("string"));
					isString = true;
				} else {
					if (/\bdef\b/.test(words[wordIndex])) { //function def
						wordIndex += 2; //skip function return type
						functions.push(words[wordIndex]);
						tokensBuilder.push(lineIndex, wordIndexes[wordIndex], words[wordIndex].length, getTokenType("function"));
						console.log(wordIndex, words[wordIndex]);

						if (line.length > 3) {
							wordIndex++; //skip with keyword

							for (wordIndex += 2; wordIndex < words.length; wordIndex += 2) { //args
								funcArgs.push(words[wordIndex]);
								tokensBuilder.push(lineIndex, wordIndexes[wordIndex], words[wordIndex].length, getTokenType("parameter"));
							}
						}
						continue;
					}

					else if (/\benddef\b/.test(words[wordIndex])) { //enddef -> reset funcArgs
						funcArgs = [];
					}

					else if (/\b(bool|int|float|str|list)\b/.test(words[wordIndex])) { //var def
						wordIndex++;
						vars.push(words[wordIndex]);
						tokensBuilder.push(lineIndex, wordIndexes[wordIndex], words[wordIndex].length, getTokenType("variable"));
					}
					
					else if (funcArgs.includes(words[wordIndex])) { //function args
						tokensBuilder.push(lineIndex, wordIndexes[wordIndex], words[wordIndex].length, getTokenType("parameter"));
					}

					else if (vars.includes(words[wordIndex])) { //var
						tokensBuilder.push(lineIndex, wordIndexes[wordIndex], words[wordIndex].length, getTokenType("variable"));
					}
					
					else if (functions.includes(words[wordIndex])) { //functions
						tokensBuilder.push(lineIndex, wordIndexes[wordIndex], words[wordIndex].length, getTokenType("function"));
					}
				}
				if (words[wordIndex].endsWith('"')) { isString = false; }
			}
		}

		return tokensBuilder.build();
	}
};

function getTokenType(tokenType: string): number {
	let result = 0;
	if (tokenTypes.has(tokenType)) {
		result = tokenTypes.get(tokenType)!;
	}
	return result;
}

const selector = { language: 'w', scheme: 'file' }; // register for all WordLang documents from the local file system

export function activate(context: vscode.ExtensionContext) {
	context.subscriptions.push(vscode.languages.registerDocumentSemanticTokensProvider(selector, provider, legend));
}
