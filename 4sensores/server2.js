const express = require('express');
const bodyParser = require('body-parser');
const mongoose = require('mongoose');

const app = express();
const port = 3000;

mongoose.connect('mongodb+srv://brunosilva576:1234awsd@brunosilvacluster.bsp3ij4.mongodb.net/laravelmongo?retryWrites=true&w=majority', {
  useNewUrlParser: true,
  useUnifiedTopology: true
});
const db = mongoose.connection;

db.on('error', console.error.bind(console, 'Erro na conexão com o MongoDB:'));
db.once('open', () => {
  console.log('Conectado ao MongoDB');
});

// Modify the schema to include all parking spaces
const estacionamentoSchema = new mongoose.Schema({
  nome: String,
  vagas: {
    "0,0": {
      Status: Number
    },
    "0,1": {
      Status: Number
    },
    "0,2": {
      Status: Number
    },
    "0,3": {
      Status: Number
    }
    // Add more spaces as needed
  }
});

const Estacionamento = mongoose.model('Estacionamento', estacionamentoSchema);

app.use(bodyParser.json());

// General route to update the status of any parking space
app.post('/atualizar-vagas', async (req, res) => {
  try {
    const { vagasOcupadas } = req.body;

    if (vagasOcupadas && Array.isArray(vagasOcupadas) && vagasOcupadas.length === 4) {
      const updatedDoc = await Estacionamento.findOneAndUpdate(
        { nome: 'EstacionaMais' },
        {
          $set: {
            'vagas.0,0.Status': vagasOcupadas[0],
            'vagas.0,1.Status': vagasOcupadas[1],
            'vagas.0,2.Status': vagasOcupadas[2],
            'vagas.0,3.Status': vagasOcupadas[3],
          }
        },
        { new: true }
      );

      console.log('Campos "Status" atualizados com sucesso:', updatedDoc);
      res.status(200).send('Campos "Status" atualizados com sucesso.');
    } else {
      res.status(400).send('JSON inválido ou array "vagasOcupadas" ausente ou com tamanho incorreto.');
    }
  } catch (error) {
    console.error('Erro ao atualizar os campos "Status":', error);
    res.status(500).send('Erro ao atualizar os campos "Status".');
  }
});

app.listen(port, '0.0.0.0', () => {
  console.log(`Servidor ouvindo em http://localhost:${port}`);
});
