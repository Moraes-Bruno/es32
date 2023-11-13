const express = require('express');
const bodyParser = require('body-parser');
const mongoose = require('mongoose');

const app = express();
const port = 3000;

// Conecte-se ao MongoDB
mongoose.connect('mongodb+srv://brunosilva576:1234awsd@brunosilvacluster.bsp3ij4.mongodb.net/EstacionaMais?retryWrites=true&w=majority', {
  useNewUrlParser: true,
  useUnifiedTopology: true
});
const db = mongoose.connection;

db.on('error', console.error.bind(console, 'Erro na conexão com o MongoDB:'));
db.once('open', () => {
  console.log('Conectado ao MongoDB');
});

// Defina um modelo de dados para a collection "Estacionamento"
const estacionamentoSchema = new mongoose.Schema({
  nome: String,
  vagas: {
    "0,0": {
      Status: Number
    }
    // Adicione outras vagas aqui, se necessário
  }
});

const Estacionamento = mongoose.model('Estacionamento', estacionamentoSchema);

app.use(bodyParser.json());

// Rota para atualizar o campo "Status" da vaga "0,0" com um JSON
app.post('/atualizar-vaga', async (req, res) => {
  try {
    const { vagaOcupada } = req.body;

    if (typeof vagaOcupada !== 'undefined') {
      const updatedDoc = await Estacionamento.findOneAndUpdate(
        { nome: 'EstacionaMais' },
        { $set: { 'vagas.0,0.Status': vagaOcupada } },
        { new: true }
      );

      console.log('Campo "Status" atualizado com sucesso:', updatedDoc);
      res.status(200).send('Campo "Status" atualizado com sucesso.');
    } else {
      res.status(400).send('JSON inválido ou campo "vagaOcupada" ausente.');
    }
  } catch (error) {
    console.error('Erro ao atualizar o campo "Status":', error);
    res.status(500).send('Erro ao atualizar o campo "Status".');
  }
});

app.listen(port, '0.0.0.0', () => {
  console.log(`Servidor ouvindo em http://localhost:${port}`);
});
