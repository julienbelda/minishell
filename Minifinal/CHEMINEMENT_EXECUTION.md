# Cheminement d'Exécution - Minishell École 42

## Table des Matières

1. [Point d'Entrée - main()](#point-dentrée---main)
2. [Boucle Principale - exec_loop()](#boucle-principale---exec_loop)
3. [Lecture de la Ligne - readline()](#lecture-de-la-ligne---readline)
4. [Analyse Lexicale - ft_lexer()](#analyse-lexicale---ft_lexer)
5. [Analyse Syntaxique - parse_tokens()](#analyse-syntaxique---parse_tokens)
6. [Exécution - ft_execute()](#exécution---ft_execute)
7. [Traitement des Heredocs - process_heredocs()](#traitement-des-heredocs---process_heredocs)
8. [Exécution du Pipeline - exec_pipeline()](#exécution-du-pipeline---exec_pipeline)
9. [Exécution d'une Commande - exec_pipeline_cmd()](#exécution-dune-commande---exec_pipeline_cmd)
10. [Lancement des Processus - launch_child()](#lancement-des-processus---launch_child)
11. [Gestion des Builtins - run_builtin()](#gestion-des-builtins---run_builtin)
12. [Commandes Externes - exec_external()](#commandes-externes---exec_external)
13. [Gestion des Erreurs](#gestion-des-erreurs)
14. [Nettoyage et Sortie](#nettoyage-et-sortie)

---

## Point d'Entrée - main()

### Code Source
```c
int main(int argc, char **argv, char **envp)
{
    t_minishell ms;
    
    if (argc != 1)
        return (1);
    
    ft_init_minishell(&ms, envp);
    ft_setup_prompt_signals();
    exec_loop(&ms);
    ft_cleanup_minishell(&ms);
    return (0);
}
```

### Cheminement d'Exécution

#### 1. Vérification des Arguments
- **Condition** : `argc != 1`
- **Action si vrai** : Retourne `1` (erreur)
- **Action si faux** : Continue l'exécution
- **Raison** : Le minishell ne doit pas recevoir d'arguments

#### 2. Initialisation du Shell
- **Fonction** : `ft_init_minishell(&ms, envp)`
- **Actions** :
  - Copie de l'environnement dans `ms.env`
  - Initialisation de `ms.last_status` à `0`
  - Configuration des variables d'environnement

#### 3. Configuration des Signaux
- **Fonction** : `ft_setup_prompt_signals()`
- **Actions** :
  - SIGINT → `ft_sigint_prompt()` (affiche nouveau prompt)
  - SIGQUIT → Ignoré
  - Autres signaux → Comportement par défaut

#### 4. Lancement de la Boucle Principale
- **Fonction** : `exec_loop(&ms)`
- **Rôle** : Boucle infinie jusqu'à `exit`

#### 5. Nettoyage Final
- **Fonction** : `ft_cleanup_minishell(&ms)`
- **Actions** : Libération de toute la mémoire allouée

---

## Boucle Principale - exec_loop()

### Code Source
```c
void exec_loop(t_minishell *ms)
{
    char *line;
    t_token *tokens;
    t_command *commands;
    int err;

    while (1)
    {
        line = readline("minishell$ ");
        if (!line)
            break;
        
        if (ft_strlen(line) == 0)
        {
            free(line);
            continue;
        }
        
        add_history(line);
        tokens = ft_lexer(line, ms);
        if (!tokens)
        {
            free(line);
            continue;
        }
        
        commands = parse_tokens(tokens, &err);
        ft_token_clear(&tokens);
        free(line);
        
        if (commands)
        {
            ft_execute(commands, ms);
            free_cmd_list(commands);
        }
    }
}
```

### Cheminement d'Exécution

#### 1. Lecture de la Ligne
- **Fonction** : `readline("minishell$ ")`
- **Possibilités** :
  - **Succès** : Retourne la ligne lue
  - **EOF (Ctrl+D)** : Retourne `NULL` → Sortie de la boucle
  - **Erreur** : Retourne `NULL` → Sortie de la boucle

#### 2. Vérification de la Ligne
- **Condition** : `ft_strlen(line) == 0`
- **Action si vrai** : Libère la ligne et continue (ligne vide)
- **Action si faux** : Continue le traitement

#### 3. Ajout à l'Historique
- **Fonction** : `add_history(line)`
- **Rôle** : Sauvegarde la ligne pour la navigation avec les flèches

#### 4. Analyse Lexicale
- **Fonction** : `ft_lexer(line, ms)`
- **Possibilités** :
  - **Succès** : Retourne la liste de tokens
  - **Erreur** : Retourne `NULL` → Continue la boucle

#### 5. Analyse Syntaxique
- **Fonction** : `parse_tokens(tokens, &err)`
- **Possibilités** :
  - **Succès** : Retourne l'arbre de commandes
  - **Erreur** : Retourne `NULL` → Continue la boucle

#### 6. Exécution
- **Condition** : `commands != NULL`
- **Action si vrai** : `ft_execute(commands, ms)`
- **Action si faux** : Continue la boucle

#### 7. Nettoyage
- **Actions** :
  - `ft_token_clear(&tokens)` : Libère les tokens
  - `free(line)` : Libère la ligne
  - `free_cmd_list(commands)` : Libère les commandes

---

## Lecture de la Ligne - readline()

### Fonctionnement
```c
char *line = readline("minishell$ ");
```

### Possibilités de Retour

#### 1. Ligne Valide
- **Exemple** : `"echo hello world"`
- **Action** : Traitement normal de la ligne

#### 2. Ligne Vide
- **Exemple** : `""` (juste Enter)
- **Action** : Affichage du prompt suivant

#### 3. EOF (Ctrl+D)
- **Retour** : `NULL`
- **Action** : Sortie propre du shell
- **Message** : Aucun (sortie silencieuse)

#### 4. Erreur de Lecture
- **Retour** : `NULL`
- **Action** : Sortie du shell
- **Cause** : Erreur système (rare)

### Gestion des Signaux Pendant la Lecture

#### SIGINT (Ctrl+C)
- **Comportement** : Interrompt la lecture
- **Action** : Affiche un nouveau prompt
- **Code de retour** : Met `ms->last_status = 130`

#### SIGQUIT (Ctrl+\)
- **Comportement** : Ignoré
- **Action** : Aucune

---

## Analyse Lexicale - ft_lexer()

### Code Source (Simplifié)
```c
t_token *ft_lexer(char *line, t_minishell *ms)
{
    t_lexctx ctx;
    t_token *tokens;
    
    ft_init_lexctx(&ctx, line);
    tokens = NULL;
    
    while (ctx.line[ctx.i])
    {
        if (ft_isspace(ctx.line[ctx.i]))
            ft_lex_space(&ctx, &tokens);
        else if (ctx.line[ctx.i] == '\'')
            ft_lex_quote_toggle(&ctx.state, ctx.line, &ctx.i);
        else if (ctx.line[ctx.i] == '"')
            ft_lex_quote_toggle(&ctx.state, ctx.line, &ctx.i);
        else if (ctx.line[ctx.i] == '$' && ctx.state != IN_SQUOTE)
            ft_lex_variable(&ctx.buf, ctx.line, &ctx.i, ms);
        else if (ft_is_operator(ctx.line[ctx.i]))
            ft_lex_operator(&ctx, &tokens);
        else
            ft_lex_char(&ctx);
    }
    
    ft_lex_flush(&ctx, &tokens);
    return (tokens);
}
```

### Cheminement d'Exécution

#### 1. Initialisation
- **Fonction** : `ft_init_lexctx(&ctx, line)`
- **Actions** :
  - Copie de la ligne
  - Initialisation de l'état à `NORMAL`
  - Initialisation du buffer dynamique

#### 2. Parcours Caractère par Caractère

##### Espaces
- **Condition** : `ft_isspace(ctx.line[ctx.i])`
- **Action** : `ft_lex_space(&ctx, &tokens)`
- **Résultat** : Flush du buffer si non vide

##### Guillemets Simples
- **Condition** : `ctx.line[ctx.i] == '\'`
- **Action** : `ft_lex_quote_toggle(&ctx.state, ctx.line, &ctx.i)`
- **Résultat** : Bascule entre `NORMAL` et `IN_SQUOTE`

##### Guillemets Doubles
- **Condition** : `ctx.line[ctx.i] == '"'`
- **Action** : `ft_lex_quote_toggle(&ctx.state, ctx.line, &ctx.i)`
- **Résultat** : Bascule entre `NORMAL` et `IN_DQUOTE`

##### Variables d'Environnement
- **Condition** : `ctx.line[ctx.i] == '$' && ctx.state != IN_SQUOTE`
- **Action** : `ft_lex_variable(&ctx.buf, ctx.line, &ctx.i, ms)`
- **Résultat** : Expansion de la variable

##### Opérateurs
- **Condition** : `ft_is_operator(ctx.line[ctx.i])`
- **Action** : `ft_lex_operator(&ctx, &tokens)`
- **Résultat** : Création d'un token d'opérateur

##### Autres Caractères
- **Action** : `ft_lex_char(&ctx)`
- **Résultat** : Ajout au buffer

#### 3. Finalisation
- **Fonction** : `ft_lex_flush(&ctx, &tokens)`
- **Action** : Flush du buffer final

### Exemples de Tokenisation

#### Exemple 1 : Commande Simple
```
Entrée : "echo hello world"
Tokens : [WORD:"echo"] [WORD:"hello"] [WORD:"world"]
```

#### Exemple 2 : Avec Variables
```
Entrée : "echo $HOME"
Tokens : [WORD:"echo"] [WORD:"/Users/julien"]
```

#### Exemple 3 : Avec Pipes
```
Entrée : "echo hello | wc -c"
Tokens : [WORD:"echo"] [WORD:"hello"] [PIPE] [WORD:"wc"] [WORD:"-c"]
```

#### Exemple 4 : Avec Redirections
```
Entrée : "echo test > output.txt"
Tokens : [WORD:"echo"] [WORD:"test"] [REDIR_OUT] [WORD:"output.txt"]
```

---

## Analyse Syntaxique - parse_tokens()

### Code Source (Simplifié)
```c
t_command *parse_tokens(t_token *tok, int *err)
{
    t_command *head;
    t_command *cur;
    
    *err = 0;
    head = ft_init_cmd();
    if (!head)
        return (NULL);
    
    cur = head;
    while (tok)
    {
        if (process_token(&cur, &tok, err) == -1)
            return (NULL);
    }
    
    if (!cur->name)
        return (head);
    
    return (head);
}
```

### Cheminement d'Exécution

#### 1. Initialisation
- **Fonction** : `ft_init_cmd()`
- **Actions** :
  - Alloue une nouvelle commande
  - Initialise tous les champs à `NULL` ou `0`
  - Définit les capacités initiales des tableaux

#### 2. Parcours des Tokens
- **Boucle** : `while (tok)`
- **Action** : `process_token(&cur, &tok, err)`

#### 3. Traitement des Tokens

##### Tokens WORD
```c
static int handle_word_token(t_command *cur, t_token **tok)
{
    if (!cur->name)
        cur->name = ft_strdup((*tok)->value);
    ft_argv_push(cur, (*tok)->value);
    *tok = (*tok)->next;
    return (0);
}
```

**Possibilités** :
- **Premier mot** : Devient le nom de la commande
- **Mots suivants** : Deviennent des arguments

##### Tokens PIPE
```c
static int handle_pipe_token(t_command **cur, t_token **tok, int *err)
{
    (*cur)->next = ft_init_cmd();
    if (!(*cur)->next)
    {
        *err = 1;
        return (-1);
    }
    *cur = (*cur)->next;
    *tok = (*tok)->next;
    return (0);
}
```

**Possibilités** :
- **Succès** : Crée une nouvelle commande
- **Erreur** : Échec d'allocation mémoire

##### Tokens REDIR
```c
static int handle_redir_token(t_command *cur, t_token **tok, int *err)
{
    t_redirection *redir;
    
    redir = malloc(sizeof(t_redirection));
    if (!redir)
    {
        *err = 1;
        return (-1);
    }
    
    redir->type = (*tok)->type;
    *tok = (*tok)->next;
    redir->file = ft_strdup((*tok)->value);
    redir->next = cur->redir;
    cur->redir = redir;
    *tok = (*tok)->next;
    return (0);
}
```

**Possibilités** :
- **Succès** : Ajoute la redirection à la commande
- **Erreur** : Échec d'allocation mémoire

#### 4. Validation Finale
- **Condition** : `!cur->name`
- **Action si vrai** : Retourne la liste (commande vide)
- **Action si faux** : Retourne la liste

### Exemples de Parsing

#### Exemple 1 : Commande Simple
```
Tokens : [WORD:"echo"] [WORD:"hello"]
Résultat : 
Command {
    name: "echo"
    argv: ["echo", "hello"]
    argc: 2
    redir: NULL
    next: NULL
}
```

#### Exemple 2 : Avec Pipe
```
Tokens : [WORD:"echo"] [WORD:"hello"] [PIPE] [WORD:"wc"] [WORD:"-c"]
Résultat :
Command1 {
    name: "echo"
    argv: ["echo", "hello"]
    next: Command2
}
Command2 {
    name: "wc"
    argv: ["wc", "-c"]
    next: NULL
}
```

#### Exemple 3 : Avec Redirection
```
Tokens : [WORD:"echo"] [WORD:"test"] [REDIR_OUT] [WORD:"file.txt"]
Résultat :
Command {
    name: "echo"
    argv: ["echo", "test"]
    redir: {
        type: REDIR_OUT
        file: "file.txt"
        next: NULL
    }
}
```

---

## Exécution - ft_execute()

### Code Source
```c
int ft_execute(t_command *cmds, t_minishell *ms)
{
    int status;
    
    if (process_heredocs(cmds, ms) == -1)
        return (-1);
    
    status = exec_pipeline(cmds, ms);
    ms->last_status = status;
    return (status);
}
```

### Cheminement d'Exécution

#### 1. Traitement des Heredocs
- **Fonction** : `process_heredocs(cmds, ms)`
- **Possibilités** :
  - **Succès** : Continue l'exécution
  - **Erreur** : Retourne `-1` (interruption par signal)

#### 2. Exécution du Pipeline
- **Fonction** : `exec_pipeline(cmds, ms)`
- **Résultat** : Code de retour de la dernière commande

#### 3. Mise à Jour du Statut
- **Action** : `ms->last_status = status`
- **Rôle** : Sauvegarde pour les commandes suivantes

---

## Traitement des Heredocs - process_heredocs()

### Code Source (Simplifié)
```c
int process_heredocs(t_command *cmds, t_minishell *ms)
{
    t_command *cmd;
    
    cmd = cmds;
    while (cmd)
    {
        if (process_command_heredocs(cmd, ms) == -1)
            return (-1);
        cmd = cmd->next;
    }
    return (0);
}
```

### Cheminement d'Exécution

#### 1. Parcours des Commandes
- **Boucle** : `while (cmd)`
- **Action** : `process_command_heredocs(cmd, ms)`

#### 2. Traitement des Redirections HEREDOC
```c
static int process_command_heredocs(t_command *cmd, t_minishell *ms)
{
    t_redirection *redir;
    
    redir = cmd->redir;
    while (redir)
    {
        if (redir->type == HEREDOC)
        {
            if (process_single_heredoc(redir, ms) == -1)
                return (-1);
        }
        redir = redir->next;
    }
    return (0);
}
```

#### 3. Exécution d'un Heredoc
```c
static int process_single_heredoc(t_redirection *r, t_minishell *ms)
{
    int fd;
    bool expand;
    
    expand = (strchr(r->file, '\'') == NULL && strchr(r->file, '"') == NULL);
    fd = ft_exec_heredoc(r->file, &fd, ms, expand);
    if (fd == -1)
        return (-1);
    
    r->fd = fd;
    return (0);
}
```

### Possibilités d'Exécution

#### 1. Heredoc Simple
```
Entrée : cat << EOF
> ligne 1
> ligne 2
> EOF
Résultat : Fichier temporaire avec le contenu
```

#### 2. Heredoc avec Variables
```
Entrée : cat << EOF
> $HOME
> $PATH
> EOF
Résultat : Fichier temporaire avec les variables expansées
```

#### 3. Heredoc sans Expansion
```
Entrée : cat << 'EOF'
> $HOME
> $PATH
> EOF
Résultat : Fichier temporaire sans expansion
```

#### 4. Interruption par Signal
```
Action : Ctrl+C pendant la lecture
Résultat : Retour -1, arrêt de l'exécution
```

---

## Exécution du Pipeline - exec_pipeline()

### Code Source (Simplifié)
```c
static int exec_pipeline(t_command *cmds, t_minishell *ms)
{
    t_command *cmd;
    int in_fd;
    int pipefd[2];
    int status;
    
    in_fd = STDIN_FILENO;
    cmd = cmds;
    
    while (cmd)
    {
        if (open_pipe_if_needed(cmd, pipefd) == -1)
            return (-1);
        
        status = exec_pipeline_cmd(cmd, ms, &in_fd, pipefd);
        if (status == -1)
            return (-1);
        
        parent_fd_maintenance(&in_fd, pipefd, cmd->next != NULL);
        cmd = cmd->next;
    }
    
    return (wait_for_children(cmds));
}
```

### Cheminement d'Exécution

#### 1. Initialisation
- **Variable** : `in_fd = STDIN_FILENO`
- **Rôle** : Descripteur d'entrée pour la première commande

#### 2. Parcours des Commandes
- **Boucle** : `while (cmd)`
- **Actions** :
  1. Ouverture du pipe si nécessaire
  2. Exécution de la commande
  3. Maintenance des descripteurs
  4. Passage à la commande suivante

#### 3. Ouverture des Pipes
```c
static int open_pipe_if_needed(t_command *c, int pipefd[2])
{
    if (c->next)
        return (pipe(pipefd));
    return (0);
}
```

**Possibilités** :
- **Commande unique** : Pas de pipe
- **Pipeline** : Ouverture d'un pipe

#### 4. Maintenance des Descripteurs
```c
static void parent_fd_maintenance(int *in_fd, int pipefd[2], bool has_next)
{
    if (has_next)
    {
        close(pipefd[1]);
        *in_fd = pipefd[0];
    }
}
```

**Actions** :
- Ferme le descripteur d'écriture du pipe
- Met à jour le descripteur d'entrée pour la commande suivante

#### 5. Attente des Processus
- **Fonction** : `wait_for_children(cmds)`
- **Rôle** : Attend la fin de tous les processus enfants
- **Retour** : Code de retour de la dernière commande

### Exemples d'Exécution

#### Exemple 1 : Commande Unique
```
Command: echo hello
Processus: 1 processus enfant
Pipe: Aucun
Résultat: Affichage direct
```

#### Exemple 2 : Pipeline Simple
```
Command: echo hello | wc -c
Processus: 2 processus enfants
Pipe: 1 pipe entre les processus
Résultat: Sortie de wc -c
```

#### Exemple 3 : Pipeline Complexe
```
Command: echo hello world | wc -w | wc -c
Processus: 3 processus enfants
Pipe: 2 pipes
Résultat: Nombre de caractères du nombre de mots
```

---

## Exécution d'une Commande - exec_pipeline_cmd()

### Code Source (Simplifié)
```c
static int exec_pipeline_cmd(t_command *cmd, t_minishell *ms, int *in_fd, int pipefd[2])
{
    int out_fd;
    pid_t pid;
    
    out_fd = (cmd->next) ? pipefd[1] : STDOUT_FILENO;
    
    if (is_builtin(cmd) && !cmd->next && *in_fd == STDIN_FILENO && !cmd->redir)
    {
        return (run_builtin(cmd, ms));
    }
    
    pid = launch_child(cmd, *in_fd, out_fd, ms);
    if (pid == -1)
        return (-1);
    
    return (0);
}
```

### Cheminement d'Exécution

#### 1. Détermination de la Sortie
- **Condition** : `cmd->next`
- **Action si vrai** : `out_fd = pipefd[1]` (pipe)
- **Action si faux** : `out_fd = STDOUT_FILENO` (sortie standard)

#### 2. Vérification Builtin
- **Condition** : `is_builtin(cmd) && !cmd->next && *in_fd == STDIN_FILENO && !cmd->redir`
- **Action si vrai** : Exécution dans le processus parent
- **Action si faux** : Lancement d'un processus enfant

#### 3. Exécution Builtin
- **Fonction** : `run_builtin(cmd, ms)`
- **Avantages** :
  - Plus rapide (pas de fork)
  - Peut modifier l'environnement
  - Accès direct aux variables du shell

#### 4. Lancement Processus Enfant
- **Fonction** : `launch_child(cmd, *in_fd, out_fd, ms)`
- **Processus** :
  1. Fork du processus
  2. Configuration des descripteurs
  3. Exécution de la commande

### Possibilités d'Exécution

#### 1. Builtin en Processus Parent
```
Condition: echo hello (sans pipe, sans redirection)
Résultat: Exécution directe, modification de l'environnement possible
```

#### 2. Builtin en Processus Enfant
```
Condition: echo hello | wc -c (avec pipe)
Résultat: Fork nécessaire pour le pipe
```

#### 3. Commande Externe
```
Condition: ls -la
Résultat: Toujours en processus enfant
```

#### 4. Commande avec Redirection
```
Condition: echo test > file.txt
Résultat: Fork nécessaire pour la redirection
```

---

## Lancement des Processus - launch_child()

### Code Source (Simplifié)
```c
pid_t launch_child(t_command *cmd, int in_fd, int out_fd, t_minishell *ms)
{
    pid_t pid;
    
    pid = fork();
    if (pid == -1)
        return (-1);
    
    if (pid == 0)
    {
        child_process(cmd, in_fd, out_fd, ms);
        exit(1);
    }
    
    return (pid);
}
```

### Cheminement d'Exécution

#### 1. Fork du Processus
- **Fonction** : `fork()`
- **Possibilités** :
  - **Succès** : Retourne le PID du processus enfant
  - **Erreur** : Retourne `-1`

#### 2. Code du Processus Enfant
- **Condition** : `pid == 0`
- **Action** : `child_process(cmd, in_fd, out_fd, ms)`
- **Sortie** : `exit(1)` en cas d'erreur

#### 3. Code du Processus Parent
- **Action** : Retourne le PID du processus enfant
- **Rôle** : Continue l'exécution du pipeline

### Processus Enfant - child_process()

```c
static void child_process(t_command *cmd, int in_fd, int out_fd, t_minishell *ms)
{
    ft_setup_child_signals();
    
    if (in_fd != STDIN_FILENO)
    {
        dup2(in_fd, STDIN_FILENO);
        close(in_fd);
    }
    
    if (out_fd != STDOUT_FILENO)
    {
        dup2(out_fd, STDOUT_FILENO);
        close(out_fd);
    }
    
    if (apply_redirections(cmd->redir) == -1)
        exit(1);
    
    if (is_builtin(cmd))
        exit(run_builtin(cmd, ms));
    else
        exec_external(cmd, ms);
}
```

#### 1. Configuration des Signaux
- **Fonction** : `ft_setup_child_signals()`
- **Actions** : Signaux par défaut du système

#### 2. Redirection d'Entrée
- **Condition** : `in_fd != STDIN_FILENO`
- **Action** : `dup2(in_fd, STDIN_FILENO)`

#### 3. Redirection de Sortie
- **Condition** : `out_fd != STDOUT_FILENO`
- **Action** : `dup2(out_fd, STDOUT_FILENO)`

#### 4. Application des Redirections
- **Fonction** : `apply_redirections(cmd->redir)`
- **Possibilités** :
  - **Succès** : Continue l'exécution
  - **Erreur** : `exit(1)`

#### 5. Exécution de la Commande
- **Condition** : `is_builtin(cmd)`
- **Action si vrai** : `exit(run_builtin(cmd, ms))`
- **Action si faux** : `exec_external(cmd, ms)`

---

## Gestion des Builtins - run_builtin()

### Code Source
```c
int run_builtin(t_command *cmd, t_minishell *ms)
{
    char **args;
    
    args = cmd->argv + cmd->assign_count;
    
    if (!ft_strcmp(cmd->name, "echo"))
        return (builtin_echo(args, ms));
    if (!ft_strcmp(cmd->name, "cd"))
        return (ft_cd(args, ms));
    if (!ft_strcmp(cmd->name, "pwd"))
        return (ft_pwd(args, ms));
    if (!ft_strcmp(cmd->name, "export"))
        return (ft_export(args, ms));
    if (!ft_strcmp(cmd->name, "unset"))
        return (builtin_unset(args, ms));
    if (!ft_strcmp(cmd->name, "env"))
        return (ft_env(args, ms));
    if (!ft_strcmp(cmd->name, "exit"))
        return (builtin_exit(args, ms));
    
    return (1);
}
```

### Cheminement d'Exécution

#### 1. Préparation des Arguments
- **Action** : `args = cmd->argv + cmd->assign_count`
- **Rôle** : Ignore les assignations en préfixe

#### 2. Identification du Builtin
- **Méthode** : Comparaison de chaînes avec `ft_strcmp`
- **Builtins supportés** : echo, cd, pwd, export, unset, env, exit

#### 3. Exécution du Builtin
- **Action** : Appel de la fonction correspondante
- **Retour** : Code de retour du builtin

### Détail des Builtins

#### builtin_echo()
```c
int builtin_echo(char **argv, t_minishell *ms)
{
    int i;
    bool newline;
    
    i = 1;  // Ignore le nom de la commande
    newline = true;
    
    while (argv[i] && argv[i][0] == '-' && argv[i][1] == 'n')
    {
        newline = false;
        i++;
    }
    
    while (argv[i])
    {
        printf("%s", argv[i]);
        if (argv[i + 1])
            printf(" ");
        i++;
    }
    
    if (newline)
        printf("\n");
    
    return (0);
}
```

**Possibilités** :
- **Sans option** : Affichage avec retour à la ligne
- **Avec -n** : Affichage sans retour à la ligne
- **Arguments multiples** : Séparés par des espaces

#### ft_cd()
```c
int ft_cd(char **argv, t_minishell *ms)
{
    char *path;
    char *oldpwd;
    
    if (!argv[1] || !ft_strcmp(argv[1], "~"))
        path = ft_env_get(ms->env, "HOME", 4);
    else if (!ft_strcmp(argv[1], "-"))
        path = ft_env_get(ms->env, "OLDPWD", 6);
    else
        path = argv[1];
    
    if (!path)
        return (1);
    
    oldpwd = getcwd(NULL, 0);
    if (chdir(path) == -1)
    {
        perror("cd");
        free(oldpwd);
        return (1);
    }
    
    ft_env_set(ms->env, "OLDPWD", oldpwd);
    ft_env_set(ms->env, "PWD", getcwd(NULL, 0));
    free(oldpwd);
    
    return (0);
}
```

**Possibilités** :
- **Sans argument** : Changement vers HOME
- **~** : Changement vers HOME
- **-** : Changement vers OLDPWD
- **Chemin relatif/absolu** : Changement vers le chemin

#### builtin_exit()
```c
int builtin_exit(char **argv, t_minishell *ms)
{
    int status;
    
    if (!argv[1])
        exit(ms->last_status);
    
    status = ft_atoi(argv[1]);
    if (argv[2])
    {
        printf("exit: too many arguments\n");
        return (1);
    }
    
    exit(status);
}
```

**Possibilités** :
- **Sans argument** : Sortie avec le dernier code de retour
- **Avec code** : Sortie avec le code spécifié
- **Trop d'arguments** : Erreur, pas de sortie

---

## Commandes Externes - exec_external()

### Code Source
```c
void exec_external(t_command *cmd, t_minishell *ms)
{
    char *path;
    char **envp;
    
    path = find_cmd_in_paths(cmd->name, ms->env);
    if (!path)
    {
        printf("%s: command not found\n", cmd->name);
        exit(127);
    }
    
    envp = ft_env_to_array(ms->env);
    if (!envp)
        exit(1);
    
    execve(path, cmd->argv, envp);
    perror("execve");
    free(path);
    free_envp(envp);
    exit(1);
}
```

### Cheminement d'Exécution

#### 1. Recherche de la Commande
- **Fonction** : `find_cmd_in_paths(cmd->name, ms->env)`
- **Possibilités** :
  - **Trouvée** : Retourne le chemin complet
  - **Non trouvée** : Retourne `NULL`

#### 2. Gestion de l'Erreur "Command Not Found"
- **Condition** : `!path`
- **Action** : `printf("%s: command not found\n", cmd->name)`
- **Sortie** : `exit(127)`

#### 3. Préparation de l'Environnement
- **Fonction** : `ft_env_to_array(ms->env)`
- **Rôle** : Conversion de la liste chaînée en tableau

#### 4. Exécution
- **Fonction** : `execve(path, cmd->argv, envp)`
- **Possibilités** :
  - **Succès** : Remplace le processus (ne retourne jamais)
  - **Erreur** : `perror("execve")` puis `exit(1)`

### Recherche de Commande - find_cmd_in_paths()

```c
char *find_cmd_in_paths(char *name, t_env *env)
{
    char *path_var;
    char **paths;
    char *candidate;
    int i;
    
    if (ft_strchr(name, '/'))
        return (ft_strdup(name));
    
    path_var = ft_env_get(env, "PATH", 4);
    if (!path_var)
        return (NULL);
    
    paths = ft_split(path_var, ':');
    if (!paths)
        return (NULL);
    
    i = 0;
    while (paths[i])
    {
        candidate = ft_strjoin(paths[i], "/");
        candidate = ft_strjoin_free(candidate, name);
        
        if (access(candidate, X_OK) == 0)
        {
            free_envp(paths);
            return (candidate);
        }
        
        free(candidate);
        i++;
    }
    
    free_envp(paths);
    return (NULL);
}
```

**Possibilités** :
- **Chemin absolu** : Retourne directement
- **Commande dans PATH** : Recherche dans les répertoires
- **Commande non trouvée** : Retourne `NULL`

---

## Gestion des Erreurs

### Types d'Erreurs

#### 1. Erreurs de Parsing
- **Cause** : Syntaxe incorrecte
- **Action** : Affichage d'erreur, continuation
- **Exemple** : `syntax error near unexpected token`

#### 2. Erreurs de Commande
- **Cause** : Commande inexistante
- **Action** : `command: command not found`
- **Code de retour** : `127`

#### 3. Erreurs de Fichier
- **Cause** : Fichier inexistant ou permissions insuffisantes
- **Action** : `perror()` avec le nom de la commande
- **Code de retour** : `1`

#### 4. Erreurs de Signal
- **Cause** : Interruption par signal
- **Action** : Sortie propre
- **Code de retour** : `130` (SIGINT)

### Propagation des Erreurs

#### 1. Dans le Lexer
- **Erreur d'allocation** : Retourne `NULL`
- **Erreur de syntaxe** : Retourne `NULL`

#### 2. Dans le Parser
- **Erreur d'allocation** : Retourne `NULL`
- **Erreur de syntaxe** : Retourne `NULL`

#### 3. Dans l'Exécuteur
- **Erreur de fork** : Retourne `-1`
- **Erreur de pipe** : Retourne `-1`
- **Erreur de redirection** : `exit(1)`

#### 4. Dans les Builtins
- **Erreur de validation** : Retourne `1`
- **Erreur système** : `perror()`, retourne `1`

---

## Nettoyage et Sortie

### Nettoyage de la Mémoire

#### 1. Dans la Boucle Principale
```c
ft_token_clear(&tokens);    // Libère les tokens
free(line);                 // Libère la ligne
free_cmd_list(commands);    // Libère les commandes
```

#### 2. Dans le Parser
```c
free(cmd->argv);            // Libère le tableau d'arguments
free(cmd->argv_full);       // Libère le tableau complet
free(cmd->name);            // Libère le nom de la commande
```

#### 3. Dans l'Exécuteur
```c
close(pipefd[0]);           // Ferme les descripteurs de pipe
close(pipefd[1]);
close(in_fd);               // Ferme le descripteur d'entrée
```

#### 4. Dans les Heredocs
```c
close(fd);                  // Ferme le fichier temporaire
unlink(temp_file);          // Supprime le fichier temporaire
```

### Sortie du Shell

#### 1. Sortie Normale
- **Commande** : `exit`
- **Action** : Libération de toute la mémoire
- **Code de retour** : Code spécifié ou dernier statut

#### 2. Sortie par Signal
- **Signal** : SIGINT (Ctrl+C)
- **Action** : Affichage du prompt
- **Continuation** : Oui

#### 3. Sortie par EOF
- **Action** : Ctrl+D
- **Comportement** : Sortie silencieuse
- **Code de retour** : `0`

### Finalisation

#### 1. Libération de l'Environnement
```c
ft_free_env_list(ms->env);  // Libère la liste des variables
```

#### 2. Libération de l'Historique
```c
rl_clear_history();         // Vide l'historique readline
```

#### 3. Restauration des Signaux
```c
// Retour aux signaux par défaut du système
```

---

## Résumé du Cheminement

### Flux Complet d'Exécution

```
main() → exec_loop() → readline() → ft_lexer() → parse_tokens() 
    ↓
ft_execute() → process_heredocs() → exec_pipeline() → exec_pipeline_cmd()
    ↓
launch_child() → child_process() → run_builtin() ou exec_external()
    ↓
Nettoyage → Retour à exec_loop()
```

### Points de Décision Clés

1. **Ligne vide** : Continue la boucle
2. **EOF** : Sortie du shell
3. **Signal** : Gestion selon le contexte
4. **Builtin vs Externe** : Exécution dans parent ou enfant
5. **Pipe** : Fork obligatoire
6. **Redirection** : Fork obligatoire
7. **Heredoc** : Traitement préalable
8. **Erreur** : Gestion et continuation

### Optimisations Appliquées

1. **Builtins en processus parent** : Évite le fork inutile
2. **Heredocs pré-traités** : Évite les problèmes de signaux
3. **Gestion mémoire optimisée** : Libération immédiate
4. **Signaux contextuels** : Gestion adaptée au contexte

Ce cheminement d'exécution garantit un comportement robuste et efficace du minishell, respectant les standards Unix tout en optimisant les performances. 