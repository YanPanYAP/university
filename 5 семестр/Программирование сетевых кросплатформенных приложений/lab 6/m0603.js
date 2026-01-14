const nodemailer = require("nodemailer");

async function send(message) {
  let testAccount = await nodemailer.createTestAccount();

  let transporter = nodemailer.createTransport({
    host: "smtp.ethereal.email",
    port: 587,
    secure: false,
    auth: {
      user: testAccount.user,
      pass: testAccount.pass,
    },
  });

  let info = await transporter.sendMail({
    from: `"Test" <${testAccount.user}>`,
    to: testAccount.user,
    subject: "Тестовое сообщение",
    text: message,
  });

  console.log("Сообщение: %s", nodemailer.getTestMessageUrl(info));
}

module.exports = { send };
