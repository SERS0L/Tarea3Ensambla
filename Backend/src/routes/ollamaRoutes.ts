import { Router, Request, Response } from 'express';
import { OllamaController } from '../controllers/ollama';

const router = Router();

router.post('/assembly/recognizer', async (req: Request, res: Response) => {
    const { prompt, model, language } = req.body;
    const ollamaController = new OllamaController();

    const response = await ollamaController.getOllamaMessage(prompt, model);
    res.status(200).send({
        message: 'Assembly recognizer endpoint hit',
        data: {
            response,
        }
    })
});

export default router;