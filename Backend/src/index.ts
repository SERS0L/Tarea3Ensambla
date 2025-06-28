import express from 'express';
import ollamaRoutes from "./routes/ollamaRoutes";

const app = express();
const PORT = 3000;

app.use(express.json());

app.use("/api", ollamaRoutes);

app.listen(PORT, () => {
    console.log(`Server is running on port:${PORT}`);
})