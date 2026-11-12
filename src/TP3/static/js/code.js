const header = document.getElementById("dataHeader");
const body = document.getElementById("dataBody");

fetch('/api/room')
    .then(response => response.json())
    .then(data => {
        header.innerHTML = "Raum: " + data.raumname;
    })
    .catch(err => console.error(err));

fetch('/api/current')
    .then(response => response.json())
    .then(data => {

        const time = new Date(data.timestamp);
        header.innerText = header.innerText + " Uhrzeit: " + time.toLocaleDateString("de-de", {
            year: 'numeric', month: 'long', day: 'numeric', hour: '2-digit', minute: '2-digit', second: '2-digit'
        });

        for (const [key, value] of Object.entries(data)) {
            const newSpan = document.createElement('span');
            newSpan.textContent = `${key}: ${value}\r`;
            body.appendChild(newSpan);
        }

    })

    .catch(err => console.error(err));