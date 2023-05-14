document.getElementById("check").addEventListener("submit", function(event) {
    event.preventDefault();
    const formData = new FormData(event.target);
    const host = formData.get("Host");
    const data = {host:host,options:{}}
    fetch("/", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(data)
    })
    .then(response => response.json())
    .then(data => {
        textArea = document.getElementById("result").textContent = data.passed ? data.statusCode : data.message
    })
    .catch(error => console.error(error));
});