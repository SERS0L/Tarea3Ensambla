import ollama, { Ollama } from "ollama";

export class OllamaController {
    private ollama;

    constructor() {
        this.ollama = new Ollama({host: "http://127.0.0.1:11434"}); // Ensure the Ollama server is running at this address
    }

    public async getOllamaMessage(prompt: string, model: string): Promise<any> {
        try {
            const response = await ollama.chat({
                model: model,
                messages: [{role: "user", content: prompt}],
            });
            console.log("response: ", response.message.content);
            return response.message.content;
        } catch (error) {
            console.error("Error in getOllamaMessage:", error);
            throw error; // Re-throw the error for further handling
        }
    }
}