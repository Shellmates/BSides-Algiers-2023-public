# chamber of secrets

## Write-up

```js
    const s = new WebSocket('ws://' + location.host + '/');
      s.addEventListener('message', e => {
        document.getElementById('note').innerHTML = e.data;
      });
      
      document.getElementById('form').onsubmit = e => {
        e.preventDefault();
        data = {
          chamber: document.getElementById('chamber').value,
          secret: document.getElementById('secret').value
        }
        s.send(JSON.stringify(data));
      }
```
check sol.py