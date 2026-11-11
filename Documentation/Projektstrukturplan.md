# Projektstrukturplan

- ## Initialisierung: 1UStd.
    - Repository anlegen
    - Rollen festlegen
    - Regeln besprechen
- ## Planung: 3UStd.
    - Planungsinstrumente verteilen & erarbeiten
    - Plichtenheft-Ergänzung
    - Projektstrukturplan
    - Zeitplan (Gantt)
    - Risikoliste
    - Schnittstellendokument v1.0 von allen TP unterschrieben
    - Technologien festlegen

- ## Kompenten implementieren: 10UStd.
    - ### TP1
        - Aufbau der Schaltung
        - Auslesen der Sensoren mit dem Arduino Nano
        - Implementation der Übersetzung in JSON
        - Übertragung an den Pi
        - Test ob Sensoren reagieren und sinnvolle Ergebnisse senden, und Schnittstelle sauber implementiert wurde

    - ### TP2
        - Empfangsdienst auf dem Pi
        - Plausibititätsprüfung
        - Datenbankentwurf
        - Übersetzung von JSON zu einer Entität
        - Persistierung
        - Test mit Fake-Daten von TP1
        - Test das ERM richtig mit dem Programm gefüllt wird

    - ### TP3
        - REST-API mit Datenbankanbindung
        - Webserver
        - Dashboard mit aktuellen Werten und Diagrammen
        - Test mit künstlichen Daten
        - REST-API erfüllt Schnittstellenvertrag

    - ### TP4
        - WLAN-Verbindung
        - Abruf der API
        - Ausgabe auf dem Display
        - Test mit Fake API


- ## Integration: 4UStd.
    - Durchgängige Kette
        - Sensor -> Nano
        - Nano -> Pi
        - Pi -> Datenbank
        - Datenbank -> REST-API
        - REST-API -> Webseite
        - REST-API -> ESP32
- ## Test und Dokumentation: 4UStd.
    - 12-Stunden-Dauertest (über Nacht)bestanden
    - Testprotokoll der Kernfälle
    - Dokumentation vollständig
- ## Abnahme: 2UStd.
    - Präsentation
    - Vorführung
    - Abnahme anhand der Checkliste
