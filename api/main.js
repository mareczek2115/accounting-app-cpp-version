const express = require('express');
const cors = require('cors');
const { json } = require('express');

const app = express();

app.use(cors());
app.use(json());

app.use(require('./get_requests'));
app.use(require('./post_requests'));
app.use(require('./delete_requests'));

app.listen(8080, () => {
  console.log('working');
});
