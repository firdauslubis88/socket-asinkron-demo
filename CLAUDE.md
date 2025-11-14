# CLAUDE.md - AI Assistant Guide for socket-asinkron-demo

## Project Overview

**Repository**: socket-asinkron-demo
**Purpose**: Demonstration project for asynchronous socket programming
**Status**: Initial setup phase
**Last Updated**: 2025-11-14

This repository serves as a demonstration and educational resource for asynchronous socket communication patterns. The project showcases various socket programming techniques including client-server architecture, real-time bidirectional communication, and event-driven programming.

---

## Repository Structure

### Current State
This is a freshly initialized repository. The following structure is recommended for development:

```
socket-asinkron-demo/
├── src/                    # Source code
│   ├── server/            # Server-side implementation
│   ├── client/            # Client-side implementation
│   └── shared/            # Shared utilities and types
├── examples/              # Example implementations
├── tests/                 # Test files
├── docs/                  # Additional documentation
├── package.json           # Node.js dependencies (if using Node.js)
├── README.md              # Project documentation
├── .gitignore            # Git ignore rules
└── CLAUDE.md             # This file
```

### Recommended Technology Stack

Based on common socket programming patterns, consider:

**Backend Options**:
- Node.js + Socket.IO (WebSocket abstraction)
- Node.js + ws (native WebSocket library)
- Python + asyncio + websockets
- Go with gorilla/websocket

**Frontend Options** (if web-based):
- Vanilla JavaScript
- React/Vue/Svelte with Socket.IO client
- TypeScript for type safety

---

## Development Workflow

### Git Branching Strategy

1. **Main Branch**: `main` or `master` - production-ready code
2. **Development Branch**: `develop` - integration branch
3. **Feature Branches**: `feature/feature-name` - specific features
4. **Claude Branches**: `claude/claude-md-*` - AI-assisted development

### Commit Conventions

Follow conventional commits format:
```
<type>(<scope>): <subject>

<body>

<footer>
```

**Types**:
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style changes (formatting)
- `refactor`: Code refactoring
- `test`: Adding or updating tests
- `chore`: Maintenance tasks

**Examples**:
```
feat(server): add WebSocket connection handling
fix(client): resolve reconnection logic issue
docs: update README with setup instructions
```

---

## Key Conventions for AI Assistants

### Code Quality Standards

1. **Error Handling**
   - Always implement proper error handling for socket connections
   - Handle disconnections gracefully
   - Implement reconnection logic with exponential backoff
   - Log errors with context for debugging

2. **Security Considerations**
   - Validate all incoming messages
   - Implement authentication/authorization if needed
   - Sanitize user inputs to prevent injection attacks
   - Use secure WebSocket (wss://) in production
   - Implement rate limiting to prevent abuse

3. **Performance**
   - Use connection pooling when appropriate
   - Implement message queuing for high-traffic scenarios
   - Consider binary protocols (e.g., MessagePack) for efficiency
   - Monitor memory usage and prevent leaks

4. **Code Style**
   - Use consistent naming conventions (camelCase for JavaScript/TypeScript)
   - Add JSDoc/TypeDoc comments for public APIs
   - Keep functions small and focused (single responsibility)
   - Use async/await over callbacks when possible

### Testing Requirements

1. **Unit Tests**
   - Test individual components in isolation
   - Mock socket connections for testing
   - Aim for >80% code coverage

2. **Integration Tests**
   - Test client-server communication
   - Verify message formats and protocols
   - Test error scenarios and edge cases

3. **End-to-End Tests**
   - Test complete user workflows
   - Verify system behavior under load
   - Test reconnection scenarios

### Documentation Standards

When adding code, always include:

1. **README Updates**
   - Installation instructions
   - Quick start guide
   - API documentation
   - Configuration options

2. **Code Comments**
   - Explain complex logic
   - Document message formats/protocols
   - Add examples for public APIs

3. **Change Documentation**
   - Update CHANGELOG.md for significant changes
   - Document breaking changes clearly
   - Provide migration guides when needed

---

## Common Patterns for Socket Programming

### 1. Connection Management

```javascript
// Example pattern for connection handling
const handleConnection = (socket) => {
  console.log(`Client connected: ${socket.id}`);

  socket.on('disconnect', (reason) => {
    console.log(`Client disconnected: ${socket.id}, Reason: ${reason}`);
  });

  socket.on('error', (error) => {
    console.error(`Socket error: ${error.message}`);
  });
};
```

### 2. Message Handling

```javascript
// Example pattern for message validation and handling
const handleMessage = (socket, message) => {
  try {
    // Validate message structure
    if (!isValidMessage(message)) {
      socket.emit('error', { message: 'Invalid message format' });
      return;
    }

    // Process message
    const response = processMessage(message);

    // Send response
    socket.emit('response', response);
  } catch (error) {
    socket.emit('error', { message: error.message });
  }
};
```

### 3. Event-Driven Architecture

```javascript
// Example event emitter pattern
class SocketManager extends EventEmitter {
  constructor() {
    super();
    this.connections = new Map();
  }

  addConnection(id, socket) {
    this.connections.set(id, socket);
    this.emit('connection:add', id);
  }

  removeConnection(id) {
    this.connections.delete(id);
    this.emit('connection:remove', id);
  }
}
```

---

## AI Assistant Guidelines

### When Adding Features

1. **Analyze First**
   - Check existing code patterns
   - Identify dependencies and conflicts
   - Review similar implementations

2. **Plan Implementation**
   - Break down into smaller tasks
   - Use TodoWrite tool for tracking
   - Consider backward compatibility

3. **Implement Incrementally**
   - Start with core functionality
   - Add tests alongside code
   - Document as you go

4. **Verify and Test**
   - Run existing tests
   - Add new test cases
   - Test edge cases and error scenarios

### When Debugging

1. **Gather Context**
   - Read error messages carefully
   - Check recent changes (git log)
   - Review related code sections

2. **Reproduce Issue**
   - Create minimal reproduction case
   - Document steps to reproduce
   - Check if issue is consistent

3. **Fix Systematically**
   - Identify root cause
   - Implement fix with tests
   - Verify fix doesn't break other functionality

### When Refactoring

1. **Ensure Test Coverage**
   - Write tests for existing behavior
   - Verify tests pass before refactoring
   - Keep tests green throughout

2. **Refactor Incrementally**
   - Make small, focused changes
   - Commit frequently
   - Test after each change

3. **Document Changes**
   - Update comments and documentation
   - Note any API changes
   - Update examples if needed

---

## Common Socket.IO Events (if using Socket.IO)

### Server-Side Events
- `connection` - New client connected
- `disconnect` - Client disconnected
- `error` - Socket error occurred

### Custom Events (Examples)
- `message` - General message exchange
- `chat:message` - Chat message
- `user:join` - User joined room
- `user:leave` - User left room
- `ping` - Heartbeat/keep-alive

### Client-Side Events
- `connect` - Connected to server
- `connect_error` - Connection error
- `disconnect` - Disconnected from server
- `reconnect` - Reconnected to server

---

## Environment Configuration

### Development
```bash
NODE_ENV=development
PORT=3000
HOST=localhost
LOG_LEVEL=debug
```

### Production
```bash
NODE_ENV=production
PORT=8080
HOST=0.0.0.0
LOG_LEVEL=info
ENABLE_CORS=false
```

---

## Troubleshooting

### Common Issues

1. **Connection Refused**
   - Check if server is running
   - Verify port is not in use
   - Check firewall settings

2. **CORS Errors**
   - Configure CORS headers on server
   - Use appropriate origin settings
   - Consider proxy for development

3. **Memory Leaks**
   - Remove event listeners on disconnect
   - Clear intervals/timeouts
   - Monitor with heap snapshots

4. **Message Loss**
   - Implement acknowledgments
   - Add message queuing
   - Use persistent storage if needed

---

## Performance Considerations

1. **Scaling**
   - Use Redis adapter for multi-server setups
   - Implement load balancing
   - Consider sticky sessions

2. **Monitoring**
   - Track active connections
   - Monitor message throughput
   - Log performance metrics

3. **Optimization**
   - Compress messages (gzip)
   - Use binary protocols when appropriate
   - Implement message batching

---

## Security Checklist

- [ ] Implement authentication
- [ ] Validate all inputs
- [ ] Use secure protocols (wss://)
- [ ] Implement rate limiting
- [ ] Add CORS configuration
- [ ] Sanitize user data
- [ ] Log security events
- [ ] Regular dependency updates
- [ ] Use environment variables for secrets
- [ ] Implement proper authorization

---

## Resources

### Documentation
- Socket.IO: https://socket.io/docs/
- WebSocket API: https://developer.mozilla.org/en-US/docs/Web/API/WebSocket
- Node.js Events: https://nodejs.org/api/events.html

### Tools
- Postman (WebSocket testing)
- wscat (CLI WebSocket client)
- Artillery (Load testing)

---

## Notes for Future Development

### Priority Tasks
1. Initialize project with appropriate package manager
2. Set up basic server implementation
3. Create simple client example
4. Add comprehensive tests
5. Write detailed README
6. Add example use cases

### Considerations
- Choose appropriate language/framework based on requirements
- Decide on message protocol (JSON, MessagePack, Protocol Buffers)
- Plan for scalability from the start
- Consider real-time vs. periodic updates
- Think about offline support and reconnection

---

## Changelog

### 2025-11-14 - Initial Setup
- Created CLAUDE.md with comprehensive guidelines
- Established project structure recommendations
- Defined coding conventions and best practices
- Set up security and performance guidelines

---

**End of Document**

*This document should be updated whenever significant changes are made to the repository structure, conventions, or development workflow.*
