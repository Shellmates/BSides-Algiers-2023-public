const jokeContainer = document.getElementById("jokeContainer");
const getJokeBtn = document.getElementById("getJokeBtn");
const showPunchlineBtn = document.getElementById("showPunchlineBtn");

let jokeId;

getJokeBtn.addEventListener("click", async () => {
  try {
    jokeId = Math.floor(Math.random() * 10)+1;
    const res = await fetch('/jokes', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ joke: jokeId })
    });
    const data = await res.json();
    showPunchlineBtn.style.display = "block";
    jokeContainer.querySelector(".joke-punchline").style.display = "none";
    jokeContainer.style.display = "block";
    jokeContainer.querySelector(".joke-setup").textContent = data.result;
  } catch (err) {
    console.error(err);
  }
});

showPunchlineBtn.addEventListener("click", async () => {
  try {
    const res = await fetch('/jokes', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ punchline: jokeId })
    });
    const data = await res.json();
    jokeContainer.querySelector(".joke-punchline").textContent = data.result;
    jokeContainer.querySelector(".joke-punchline").style.display = "block";
    showPunchlineBtn.style.display = "none";
  } catch (err) {
    console.error(err);
  }
});
