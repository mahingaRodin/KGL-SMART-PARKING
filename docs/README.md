# Documentation Index

This folder contains technical documentation for the **Kigali Smart Parking Management System**.

## Contents

| Document | Description |
|----------|-------------|
| [System Flow](system-flow.md) | End-to-end process flows, menu navigation, and task mapping |
| [File Reference](file-reference.md) | Purpose and role of every project file |
| [Diagrams](diagrams/) | Standalone Mermaid diagram source files |

## Diagrams

| Diagram | File | What it shows |
|---------|------|---------------|
| System Architecture | [diagrams/architecture.mmd](diagrams/architecture.mmd) | Layers, components, and data stores |
| Class Diagram | [diagrams/class-diagram.mmd](diagrams/class-diagram.mmd) | OOP relationships between classes |
| Menu Flow | [diagrams/menu-flow.mmd](diagrams/menu-flow.mmd) | Console menu navigation |
| Vehicle Entry Flow | [diagrams/vehicle-entry-flow.mmd](diagrams/vehicle-entry-flow.mmd) | Slot allocation and entry registration |
| Vehicle Exit Flow | [diagrams/vehicle-exit-flow.mmd](diagrams/vehicle-exit-flow.mmd) | Fee calculation, slot release, history |
| Data Structures | [diagrams/data-structures.mmd](diagrams/data-structures.mmd) | In-memory stores and operations |

## How to View Diagrams

Mermaid diagrams render automatically on GitHub, GitLab, and in VS Code/Cursor with a Mermaid preview extension.

To export as images locally:

```bash
# Install Mermaid CLI (requires Node.js)
npm install -g @mermaid-js/mermaid-cli

# Generate PNG from any diagram
mmdc -i docs/diagrams/architecture.mmd -o docs/diagrams/architecture.png
```

## Quick Links

- Project README: [../README.md](../README.md)
- Test scenarios: [../test_input.txt](../test_input.txt)
