node {
  stage('Checkout') {
    git 'ssh://vcs-user@phabricator.dockstudios.co.uk/diffusion/SHAMEAN/shamean.git'
  }
  stage('Build') {
    sh 'g++ -lcrypto -lssl -g main.cpp -o shamean'
  }

  stage('Archive') {
    sh "zip shamean-${env.BUILD_NUMBER}" shamean
    archiveArtifacts artifacts: 'shamean*.zip,shamean', fingerprint: true
  }
  stage('CleanUP') {
    cleanWs(patterns: [[pattern: 'shamean', type: 'INCLUDE']])
  }
}