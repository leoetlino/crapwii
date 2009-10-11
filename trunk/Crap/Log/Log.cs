//-------------------------------------
// WBFSSync - WBFSSync.exe
//
// Copyright 2009 Caian (ÔmΣga Frøst) <frost.omega@hotmail.com> :
//
// WBFSSync is Licensed under the terms of the Microsoft Reciprocal License (Ms-RL)
//
// Log.cs:
//
// Gerenciador de mensagens do programa
//
//-------------------------------------

using System;
using System.Collections.Generic;
//using System.Linq;
using System.Text;
using System.IO;

namespace WBFSSync
{
    public enum LogMessageType
    {
        Message,
        Warning,
        Error,
    }

    public delegate void AppendLogLineDelegate(String line, LogMessageType messagetype);
    public delegate void LastLineChangedDelegate(String part);

    //Delegate usado para todos os objetos que quiserem transmitir um evento de Log para o programa principal,
    //para ser formatado pelo idioma
    public delegate void MessageSentDeletate(String sender, int code, object extendedinfo, LogMessageType type);
    
    public static class Log
    {
        const string LogDir = "Log\\";
        const string LogFile = LogDir + "Log.txt";
        const string ExceptionLogFile = LogDir + "Exceptions.txt";

        public static bool DisableLog = false;

        public static StringBuilder LogBuilder = null;
        public static Boolean SaveLog = true;
        public static Boolean SaveExceptionLog = true;

        public static int MaxLogSize = 131072; // 512 linhas @ 256 caracteres, o número de linhas pode variar de acordo
                                               // com o número de caracteres de cada mensagem

        public static event AppendLogLineDelegate AppendLogLine;
        public static event LastLineChangedDelegate LastLineChanged;

        public static event MessageSentDeletate ObjectMessageSent;

        public static void LogText(String text, params object[] args)
        {
            if (LogBuilder == null) return;
            String formatted = String.Format(text, args);
            try
            {
                LogBuilder.Append(formatted);
            }
            catch (ArgumentOutOfRangeException)
            {
                if (SaveLog) SaveLogToFile();
                LogBuilder.Remove(0, LogBuilder.Length);

                LogBuilder.Append(formatted);
                LogBuilder.AppendLine();
            }

            OnLogTextChanged(formatted);
        }

        public static void LogLine(String line, LogMessageType type, params object[] args)
        {
            if (LogBuilder == null) return;
            String formatted = String.Format(line, args);
            try
            {
                LogBuilder.Append(formatted);
                LogBuilder.AppendLine();
            }
            catch (ArgumentOutOfRangeException)
            {
                if (SaveLog) SaveLogToFile();
                LogBuilder.Remove(0, LogBuilder.Length);

                LogBuilder.Append(formatted);
                LogBuilder.AppendLine();
            }

            OnLogTextLine(formatted, type);
        }

        public static void LogException(Exception e)
        {
            if (LogBuilder == null) return;
            LogLine("O método {0} gerou uma exceção do tipo {1}", LogMessageType.Error, e.TargetSite, e.GetType().ToString());
            if (SaveExceptionLog) SaveExceptionToFile(e);
        }

        public static void End()
        {
            if (LogBuilder == null) return;
            if (SaveLog) SaveLogToFile();
        }

        public static void Begin()
        {
            LogBuilder = new StringBuilder(0, MaxLogSize);

            if (!Directory.Exists(LogDir)) Directory.CreateDirectory(LogDir);
            if (File.Exists(LogFile)) File.Delete(LogFile);
        }

        private static void OnLogTextLine(String line, LogMessageType type)
        {
            if (AppendLogLine != null) AppendLogLine(line, type);
        }
        
        private static void OnLogTextChanged(String piece)
        {
            if (LastLineChanged != null) LastLineChanged(piece);
        }

        private static void SaveLogToFile()
        {
            if (!Directory.Exists(LogDir)) Directory.CreateDirectory(LogDir);
            File.AppendAllText(LogFile, LogBuilder.ToString());
        }

        private static void SaveExceptionToFile(Exception e)
        {
            if (!Directory.Exists(LogDir)) Directory.CreateDirectory(LogDir);
            File.AppendAllText(ExceptionLogFile, e.GetType().ToString() + '\n' + e.TargetSite + '\n' + e.StackTrace);
        }

        public static void SendMessage(String sender, int code, object extendedinfo, LogMessageType type)
        {
            if (DisableLog) return;
            if(ObjectMessageSent != null) ObjectMessageSent(sender, code, extendedinfo, type);
        }
    }
}
