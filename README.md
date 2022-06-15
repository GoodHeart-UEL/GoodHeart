<h1 align="center">
  ❤️ Good Heart 💻
</h1>

<h4 align="center">
  <i>Sistema embarcado para detecção de anomalias cardíacas em tempo real.</i><br>
</h4>

<div align="center">
  <h4>
    <a href="#requisitos">Requisitos</a> |
    <a href="#Descrição-do-Repositório">Descrição do Repositório</a> |
    <a href="#participantes">Participantes</a>
  </h4>
</div>

## Requisitos

Para executar as aplicações corretamente, você vai precisar:
- [e² studio](https://www.renesas.com/us/en/software-tool/e-studio) (versão 2022-04 ou mais recente)
- [Synergy™ Software Package (SSP)](https://www.renesas.com/us/en/products/microcontrollers-microprocessors/renesas-synergy-platform-mcus/renesas-synergy-software-package#overview) (versão 2.2.0 ou mais recente)
- [Flutter](https://yarnpkg.com/) (versão 1.19.0 ou mais recente)

## Descrição do Repositório

O repositório é composto pela aplicação embarcada compatível com a placa Renesas SK-S7G2 e aplicativo em Flutter responsável pela comunicação com o sistema embarcado.

Para executar o projeto da placa SK-S7G2 é necessário importar o projeto contido na pasta ```GUIApp``` na IDE e² studio.

Para executar o aplicativo que se comunica com a placa SK-S7G2 é necessário estar na pasta ```APP/good_heart``` e executar o comando:
```
flutter run
```

## Participantes

- [Prof. Dr. Wesley Attrot](https://github.com/wattrot)
- [Prof. Dr. Fábio Sakuray](https://github.com/fabiosakuray)
- [João Alex Bergamo](https://github.com/joao-alex)
- [Denise Figueiredo](https://github.com/deniserezende)
- [Mateus Komarchesqui](https://github.com/MateusKomarchesqui)
