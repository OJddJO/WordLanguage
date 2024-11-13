import { start } from 'repl';
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
			const line = lines[lineIndex];
			const words = line.split(/\s+/); // splits by any whitespace
			const whitespaces = line.split(/\b.*\b/);
			let startChar = 0;
			let isString = false;

			for (let wordIndex = 0; wordIndex < words.length; wordIndex++) {
				if (words[wordIndex].startsWith('"')) { //if the word starts with " then it's a string
					tokensBuilder.push(lineIndex, startChar, words[wordIndex].length, getTokenType("string"));
					isString = true;
				} else {
					if (/\bdef\b/.test(words[wordIndex])) { //function def
						startChar += words[wordIndex].length + whitespaces[wordIndex].length;
						wordIndex++;
						functions.push(words[wordIndex]);
						tokensBuilder.push(lineIndex, startChar, words[wordIndex].length, getTokenType("function"));

						if (line.length > 2) {
							startChar += words[wordIndex].length + whitespaces[wordIndex].length; //skip with keyword
							wordIndex++;

							for (let argIndex = 3; argIndex < words.length; argIndex++) { //args
								startChar += words[wordIndex].length + whitespaces[wordIndex].length;
								wordIndex++;
								funcArgs.push(words[wordIndex]);
								tokensBuilder.push(lineIndex, startChar, words[wordIndex].length, getTokenType("parameter"));
							}
						}
						continue;
					}

					else if (/\benddef\b/.test(words[wordIndex])) { //enddef -> reset funcArgs
						funcArgs = [];
					}

					else if (/\b(bool|int|float|str|list)\b/.test(words[wordIndex])) { //var def
						startChar += words[wordIndex].length + whitespaces[wordIndex].length;
						wordIndex++;
						vars.push(words[wordIndex]);
						tokensBuilder.push(lineIndex, startChar, words[wordIndex].length, getTokenType("variable"));
					}

					else if (vars.includes(words[wordIndex])) { //var
						tokensBuilder.push(lineIndex, startChar, words[wordIndex].length, getTokenType("variable"));
					}

					else if (functions.includes(words[wordIndex])) {
						tokensBuilder.push(lineIndex, startChar, words[wordIndex].length, getTokenType("function"));
					}

					else if (funcArgs.includes(words[wordIndex])) {
						tokensBuilder.push(lineIndex, startChar, words[wordIndex].length, getTokenType("parameter"));
					}
				}
				if (words[wordIndex].endsWith('"')) { isString = false; }
				startChar += words[wordIndex].length + whitespaces[wordIndex].length;
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
