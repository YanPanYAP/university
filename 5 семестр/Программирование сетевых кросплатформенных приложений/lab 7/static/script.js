document.getElementById("js-test").textContent = "js работает";

fetch("/data.json")
  .then((r) => r.json())
  .then((data) => {
    document.getElementById("json-out").textContent = JSON.stringify(
      data,
      null,
      2
    );
  });

fetch("/data.xml")
  .then((r) => r.text())
  .then((xml) => {
    document.getElementById("xml-out").textContent = xml;
  });
